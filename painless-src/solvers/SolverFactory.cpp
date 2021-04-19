// -----------------------------------------------------------------------------
// Copyright (C) 2017  Ludovic LE FRIOUX
//
// This file is part of PaInleSS.
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

#include "../solvers/KissatSolver.h"
#include "../solvers/SolverFactory.h"
#include "../utils/Parameters.h"
#include "../utils/System.h"

void
SolverFactory::sparseRandomDiversification(
      const vector<SolverInterface *> & solvers)
{
   if (solvers.size() == 0)
      return;

   int vars = solvers[0]->getVariablesCount();

   for (int sid = 0; sid < solvers.size(); sid++) {
      srand(sid);
      for (int var = 1; var <= vars; var++) {
         if (rand() % solvers.size() == 0) {
            solvers[sid]->setPhase(var, rand() % 2 == 1);
         }
      }
   }
}

void
SolverFactory::randomDiversification(
      const vector<SolverInterface *> & solvers)
{
   if (solvers.size() == 0)
      return;

   for (int sid = 0; sid < solvers.size(); sid++) {
       solvers[sid]->randomDiversify(sid);
   }
}

void
SolverFactory::singleNativeAndRandomDiversification(const vector<SolverInterface *> & solvers, int confID)
{
    
    vector<SolverInterface *> solversWithRandDiv;
    
    for (int sid = 0; sid < solvers.size(); sid++) {
        if (sid != confID) solversWithRandDiv.push_back(solvers[sid]);
    }
    //Random diversification is applied except for one solver, that with the same id as confID
    randomDiversification(solversWithRandDiv);
    
    
    for (int sid = 0; sid < solvers.size(); sid++) {
       solvers[sid]->diversify(confID);
    }
   
}
void
SolverFactory::nativeDiversification(const vector<SolverInterface *> & solvers)
{
   for (int sid = 0; sid < solvers.size(); sid++) {
      solvers[sid]->diversify(sid);
   }
}

SolverInterface *
SolverFactory::createKissatSolver()
{
   int id = currentIdSolver.fetch_add(1);

   SolverInterface * solver = new KissatSolver(id);

//   solver->loadFormula(Parameters::getFilename());

   return solver;
}

void
SolverFactory::createKissatSolvers(int maxSolvers,
                                  vector<SolverInterface *> & solvers)
{
   solvers.push_back(createKissatSolver());

//   double memoryUsed    = getMemoryUsed();
//   int maxMemorySolvers = Parameters::getIntParam("max-memory", 51) * 1024 *
//                          1024 / memoryUsed;
//
//   if (maxSolvers > maxMemorySolvers) {
//      maxSolvers = maxMemorySolvers;
//   }

   for (int i = 1; i < maxSolvers; i++) {
      solvers.push_back(createKissatSolver());
   }
}

void
SolverFactory::printStats(const vector<SolverInterface *> & solvers)
{
   printf("c | ID | conflicts  | propagations |  restarts  | decisions  " \
          "| importedClauses | importedUnits| exportedClauses | exportedUnits|\n");

   for (size_t i = 0; i < solvers.size(); i++) {
      SolvingStatistics stats = solvers[i]->getStatistics();

      printf("c | %2zu | %10ld | %12ld | %10ld | %10ld | %15ld | %12ld | %15ld | %12ld | \n",
             solvers[i]->id, stats.conflicts, stats.propagations,
             stats.restarts, stats.decisions, stats.nbImportedClauses,stats.nbImportedUnits
              , stats.nbExportedClauses,stats.nbExportedUnits);
   }
}
