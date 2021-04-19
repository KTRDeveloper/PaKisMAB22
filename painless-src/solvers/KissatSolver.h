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

#pragma once

#include "../clauses/ClauseBuffer.h"
#include "../solvers/SolverInterface.h"
#include "../utils/Threading.h"
#include "../../kissat/src/kissat.h"    


using namespace std;

// Some forward declatarations for MapleCOMSPS
//namespace Minisat
//{
//	class SimpSolver;
//	class Lit;
//	template<class T> class vec;
//}

class CaDiCalSolverLearner;

/// Instance of a MapleCOMSPS solver
class KissatSolver : public SolverInterface
{
//    friend CaDiCalSolverLearner;
public:
   /// Load formula from a given dimacs file, return false if failed.
   bool loadFormula(const char* filename);

   /// Get the number of variables of the current resolution.
   int getVariablesCount();
   void setVariablesCount(int max_var);

   /// Get a variable suitable for search splitting.
   int getDivisionVariable();

   /// Set initial phase for a given variable.
   void setPhase(const int var, const bool phase);

   /// Bump activity of a given variable.
   void bumpVariableActivity(const int var, const int times);

   /// Interrupt resolution, solving cannot continue until interrupt is unset.
   void setSolverInterrupt();
   
   /// Remove the SAT solving interrupt request.
   void unsetSolverInterrupt();

   /// Solve the formula with a given cube.
   SatResult solve(const vector<int> & cube);

   /// Add a permanent clause to the formula.
   void addClause(ClauseExchange * clause);
   
   /// Add a list of permanent clauses to the formula.
   void addClauses(const vector<ClauseExchange *> & clauses);
   
   /// Add a list of initial clauses to the formula.
   void addInitialClauses(const vector<ClauseExchange *> & clauses);

   /// Add a learned clause to the formula.
   void addLearnedClause(ClauseExchange * clause);
   
   /// Add a list of learned clauses to the formula.
   void addLearnedClauses(const vector<ClauseExchange *> & clauses);

   /// Get a list of learned clauses.
   void getLearnedClauses(vector<ClauseExchange *> & clauses);

   /// Request the solver to produce more clauses.
   void increaseClauseProduction();
   
   /// Request the solver to produce less clauses.
   void decreaseClauseProduction();

   /// Get solver statistics.
   SolvingStatistics getStatistics();
   
   /// Return the model in case of SAT result.
   vector<int> getModel();
   
   /// Native diversification.
   void diversify(int id);
   
   // Random diversification.
   void randomDiversify(int id);

   /// Constructor.
   KissatSolver(int id);
   
   /// Destructor.
   virtual ~KissatSolver();

protected:
   /// Pointer to a Kissat solver.
   kissat * solver;

   /// Buffer used to import clauses (units included).
   ClauseBuffer clausesToImport;
   ClauseBuffer unitsToImport;

   /// Buffer used to export clauses (units included).
   ClauseBuffer clausesToExport;

   /// Buffer used to add permanent clauses.
   ClauseBuffer clausesToAdd;
   
   /// Size limit used to share clauses.
   atomic<int> lbdLimit;
   
   /// Used to stop or continue the resolution.
   atomic<bool> stopSolver;
   
   int max_var = 0;
   /// Callback to export/import clauses.
   friend int cbkKissatImportUnit(void *);
   friend bool cbkKissatImportClause(void *, int *, vector<int> &);
   friend void cbkKissatExportClause(void *, int, vector<int> &);
};

//class CaDiCalSolverLearner : CaDiCaL::Learner {
//  CaDiCalSolver * cSolver;
//  std::vector<int> clause;
//  int glue;
//public:
//  unsigned clauses;
//  CaDiCalSolverLearner (CaDiCalSolver * s) : cSolver (s), clauses (0) { cSolver->solver->connect_learner (this); }
//  ~CaDiCalSolverLearner () { cSolver->solver->disconnect_learner (); }
//  bool learning (int size, int glue) { // learning should be always called before learn
//      if (glue > cSolver->lbdLimit) return false;
//      this->glue = glue;
//      return true; 
//  }
//  void learn (int lit) {
//    if (lit) clause.push_back (lit);
//    else {
//        cbkCaDiCalExportClause(cSolver, glue, clause);
////      std::cout << "solver[" << ((void*) solver) << "] imported clause of size "
////                << clause.size () << ':';
////      for (auto lit : clause)
////	std::cout << ' ' << lit;
////      std::cout << std::endl << std::flush;
//      clause.clear ();
//      clauses++;
//    }
//  }
//};