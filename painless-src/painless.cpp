// -----------------------------------------------------------------------------
// Copyright (C) 2017  Ludovic LE FRIOUX
//
// PaInleSS is free software: you can redistribute it and/or modify it under the
// terms of the GNU General Public License as published by the Free Software
// Foundation, either version 3 of the License, or (at your option) any later
// version.
//
// This program is distributed in the hope that it will be useful, but WITHOUT
// ANY WARRANTY; without even the implied warranty of MERCHANTABILITY or FITNESS
// FOR A PARTICULAR PURPOSE.  See the GNU General Public License for more
// details.
//
// You should have received a copy of the GNU General Public License along with
// this program.  If not, see <http://www.gnu.org/licenses/>.
// -----------------------------------------------------------------------------

#include "painless.h"

#include "utils/Logger.h"
#include "utils/Parameters.h"
#include "utils/System.h"
#include "utils/SatUtils.h"

#include "solvers/SolverFactory.h"

#include "clauses/ClauseManager.h"

#include "sharing/HordeSatSharing.h"
#include "sharing/SimpleSharing.h"
#include "sharing/Sharer.h"

#include "working/SequentialWorker.h"
#include "working/Portfolio.h"

#include <unistd.h>
#include <algorithm>


using namespace std;


// -------------------------------------------
// Declaration of global variables
// -------------------------------------------
atomic<bool> globalEnding(false);

Sharer ** sharers = NULL;

int nSharers = 0;

WorkingStrategy * working = NULL;

SatResult finalResult = UNKNOWN;

vector<int> finalModel;


// -------------------------------------------
// Main of the framework
// -------------------------------------------
int main(int argc, char ** argv)
{
   Parameters::init(argc, argv);

   if (Parameters::getFilename() == NULL ||
       Parameters::getBoolParam("h"))
   {
      cout << "USAGE: " << argv[0] << " [options] input.cnf" << endl;
      cout << "Options:" << endl;
      cout << "\t-c=<INT>\t\t number of cpus, default is 24" << endl;
      cout << "\t-t=<INT>\t\t timeout in seconds, default is no limit" << endl;
      cout << "\t-v=<INT>\t\t verbosity level, default is 0" << endl;

      return 0;
   }

   Parameters::printParams();

   int cpus = Parameters::getIntParam("c", 24);
   setVerbosityLevel(Parameters::getIntParam("v", 0));

   // Create and init solvers
   vector<SolverInterface *> solvers;

   SolverFactory::createKissatSolvers(cpus, solvers);
   SolverFactory::nativeDiversification(solvers);
   
   double startLoadingTime = getRelativeTime();
   loadFormulaToSolvers(solvers, Parameters::getFilename());
   cout << "\nc Formula loading time: " << getRelativeTime() - startLoadingTime << "s" << endl;
   fflush(stdout);
   
   int nSolvers = solvers.size();

   cout << "c " << nSolvers << " solvers are used, with IDs in [|0, "
        << nSolvers - 1 << "|]." << endl;

   // Init working
   working = new Portfolio();
   for (size_t i = 0; i < nSolvers; i++) {
      working->addSlave(new SequentialWorker(solvers[i]));
   }


   // Init the management of clauses
   ClauseManager::initClauseManager();


   // Launch working
   vector<int> cube;
   working->solve(cube);


   // Wait until end or timeout
   int timeout = Parameters::getIntParam("t", -1);

   while(globalEnding == false) {
      sleep(1);

      if (timeout > 0 && getRelativeTime() >= timeout) {
         globalEnding = true;
         working->setInterrupt();
      }
   }


   // Print solver stats
   // SolverFactory::printStats(solvers);


   // Delete working strategy
   delete working;


   // Delete shared clauses
   ClauseManager::joinClauseManager();


   // Print the result and the model if SAT
   cout << "c Resolution time: " << getRelativeTime() << "s" << endl;

   if (finalResult == SAT) {
      cout << "s SATISFIABLE" << endl;

      if (Parameters::getBoolParam("no-model") == false) {
         printModel2(finalModel);
      }
   } else if (finalResult == UNSAT) {
      cout << "s UNSATISFIABLE" << endl;
   } else {
      cout << "s UNKNOWN" << endl;
   }

   return finalResult;
}
