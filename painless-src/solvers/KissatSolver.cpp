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

// MapleCOMSPS includes
//#include "../../ExMapleLCMDistChronoBT/ExMapleLCMDistChronoBT/utils/System.h"
//#include "../../ExMapleLCMDistChronoBT/ExMapleLCMDistChronoBT/core/Dimacs.h"
//#include "../../ExMapleLCMDistChronoBT/ExMapleLCMDistChronoBT/simp/SimpSolver.h"



#include "../utils/Logger.h"
#include "../utils/System.h"
#include "../utils/Parameters.h"
#include "../clauses/ClauseManager.h"
#include "../solvers/KissatSolver.h"



void cbkKissatExportClause(void * issuer, int lbd, vector<int> & cls)
{
	KissatSolver* mp = (KissatSolver*)issuer;

	if (lbd > mp->lbdLimit)
		return;

	ClauseExchange * ncls = ClauseManager::allocClause(cls.size());

	for (int i = 0; i < cls.size(); i++) {
		ncls->lits[i] = cls[i]; //INT_LIT(cls[i]);
	}

   ncls->lbd  = lbd;
   ncls->from = mp->id;

   mp->clausesToExport.addClause(ncls);
}

int cbkKissatImportUnit(void * issuer)
{
   KissatSolver * mp = (KissatSolver*)issuer;

   //Lit l = lit_Undef;
   int l = 0;
   
   ClauseExchange * cls = NULL;

   if (mp->unitsToImport.getClause(&cls) == false)
      return 0;

   l = cls->lits[0]; //MINI_LIT(cls->lits[0]);
   ClauseManager::releaseClause(cls);

   return l;
}

bool cbkKissatImportClause(void * issuer, int * lbd, vector<int> & mcls)
{
   KissatSolver* mp = (KissatSolver*)issuer;

   ClauseExchange * cls = NULL;

   if (mp->clausesToImport.getClause(&cls) == false)
      return false;

   mcls.clear();
   for (int i = 0 ; i < cls->size ; i++) mcls.push_back(cls->lits[i]);
   // makeMiniVec(cls, mcls);
   *lbd = cls->lbd;

   ClauseManager::releaseClause(cls);

   return true;
}

KissatSolver::KissatSolver(int id) : SolverInterface(id, CADICAL)
{
	lbdLimit = Parameters::getIntParam("lbd-limit", 2);

	solver = kissat_init();        
}

KissatSolver::~KissatSolver()
{
        kissat_release (solver);
}

bool
KissatSolver::loadFormula(const char* filename)
{
  
    printf("\nc Not yet implemented\n");
    exit(1);
//    gzFile in = gzopen(filename, "rb");
//
//    parse_DIMACS(in, *solver);
//
//    gzclose(in);

//    int vars =0;
//    solver->read_dimacs(filename,vars, 0);
    return true;
}

//Get the number of variables of the formula
int
KissatSolver::getVariablesCount()
{
    return max_var;
}
void
KissatSolver::setVariablesCount(int max_var)
{
    this->max_var = max_var;
}
// Get a variable suitable for search splitting
int
KissatSolver::getDivisionVariable()
{
   return (rand() % getVariablesCount()) + 1;
}

// Set initial phase for a given variable
void
KissatSolver::setPhase(const int var, const bool phase)
{
	// solver->setPolarity(var - 1, phase ? true : false);
}

// Bump activity for a given variable
void
KissatSolver::bumpVariableActivity(const int var, const int times)
{
}

// Interrupt the SAT solving, so it can be started again with new assumptions
void
KissatSolver::setSolverInterrupt()
{
   stopSolver = true;
   
   kissat_terminate(solver);

}

void
KissatSolver::unsetSolverInterrupt()
{
   stopSolver = false;

//	solver->clearInterrupt();
}

// Diversify the solver
void
KissatSolver::diversify(int id)
{
     
    switch(id){ 
        case 0 : kissat_set_configuration (solver, "default");
            break;
        case 1 : 
           kissat_set_configuration (solver, "sat");
            break;
        case 2 : 
            kissat_set_configuration (solver, "unsat");
            break;  
        case 3 : 
            //default_chrono_false
            kissat_set_configuration (solver, "default");
            kissat_set_option(solver, "chrono", false);
            break;    
        case 4 :
            //default_chrono_false_phase_false
            kissat_set_configuration (solver, "default");
            kissat_set_option(solver, "chrono", false);
            kissat_set_option(solver, "phase", false);
            
            break;
        case 5 : 
            //default_phase_false
            kissat_set_configuration (solver, "default");
            kissat_set_option(solver, "phase", false);

            break;
        case 6 : 
            //default_stable_2_chrono_false
            kissat_set_configuration (solver, "default");
            kissat_set_option(solver, "stable", 2);
            kissat_set_option(solver, "chrono", false);
            break;
        case 7 : 
            //default_target_0
            kissat_set_configuration (solver, "default");
            kissat_set_option(solver, "target", 0);

            break;
        case 8 : 
            //default_target_0_chrono_false_phase_false
            kissat_set_configuration (solver, "default");
            kissat_set_option(solver, "target", 0);
            kissat_set_option(solver, "chrono", false);
            kissat_set_option(solver, "phase", false);

            break;
        case 9 : 
            //default_target_0_phase_false
            kissat_set_configuration (solver, "default");
            kissat_set_option(solver, "target", 0);
            kissat_set_option(solver, "phase", false);

            break;
        case 10 : 
            //default_walkinitially_true
            kissat_set_configuration (solver, "default");
            kissat_set_option(solver, "walkinitially", true);

            break;
        case 11 : 
            //sat_chrono_false
            kissat_set_configuration (solver, "sat");
            kissat_set_option(solver, "chrono", false);
            
            break;
        case 12 : 
            //sat_chrono_false_phase_false
            kissat_set_configuration (solver, "sat");
            kissat_set_option(solver, "chrono", false);
            kissat_set_option(solver, "phase", false);

            break;
        case 13 : 
            //sat_chrono_false_phase_false_tier1_3
            kissat_set_configuration (solver, "sat");
            kissat_set_option(solver, "chrono", false);
            kissat_set_option(solver, "phase", false);
            kissat_set_option(solver, "tier1", 3);

            break;
        case 14 : 
            //sat_chrono_false_tier1_3
            kissat_set_configuration (solver, "sat");
            kissat_set_option(solver, "chrono", false);
            kissat_set_option(solver, "tier1", 3);

            break;
        case 15 : 
            //sat_phase_false
            kissat_set_configuration (solver, "sat");
            kissat_set_option(solver, "phase", false);

            break;
        case 16 : 
            //sat_stable_2_chrono_false
            kissat_set_configuration (solver, "sat");
            kissat_set_option(solver, "stable", 2);
            kissat_set_option(solver, "chrono", false);
            break;
        case 17 : 
            //sat_target_0 or default_target_0 (since sat \equiv default && target 2)
            kissat_set_configuration (solver, "default");
            kissat_set_option(solver, "target", 0);
            break;
        case 18 : 
            //sat_target_0_chrono_false_phase_false or default_target_0_chrono_false_phase_false (since sat \equiv default && target 2)
            kissat_set_configuration (solver, "default");
            kissat_set_option(solver, "target", 0);
            kissat_set_option(solver, "chrono", false);
            kissat_set_option(solver, "phase", false);
            break;
        case 19 : 
            //sat_target_0_phase_false or default_target_0_phase_false (since sat \equiv default && target 2)
            kissat_set_configuration (solver, "default");
            kissat_set_option(solver, "target", 0);
            kissat_set_option(solver, "phase", false);
            
            break;
        case 20 : 
            //sat_target_0_tier1_3
            kissat_set_configuration (solver, "sat");
            kissat_set_option(solver, "target", 0);
            kissat_set_option(solver, "tier1", 3);
            break;
        case 21 : 
            //sat_tier1_3
            kissat_set_configuration (solver, "sat");
            kissat_set_option(solver, "tier1", 3);

            break;
        case 22 : 
            //sat_walkinitially_true
            kissat_set_configuration (solver, "sat");
            kissat_set_option(solver, "walkinitially", true);
            break;
        case 23 : 
            //unsat_stable_2_chrono_false
            kissat_set_configuration (solver, "unsat");
            kissat_set_option(solver, "chrono", false);

            break;
        default:
            kissat_set_configuration (solver, "default");
            break;
    }
    
    int chrono = kissat_get_option(solver, "chrono");
    int stable = kissat_get_option(solver, "stable");
    int walkinitially = kissat_get_option(solver, "walkinitially");
    int target = kissat_get_option(solver, "target");
    int tier1 = kissat_get_option(solver, "tier1");
    int phase = kissat_get_option(solver, "phase");
    printf("\nc time = %f s ** id = %d / %s=%d / %s=%d / %s=%d / %s=%d / %s=%d/ %s=%d",getRelativeTime(), id, "tier1", tier1, "chrono", chrono, "stable", stable, "walkinitially", walkinitially, "target", target, "phase", phase);
    fflush(stdout);
}

// Diversify the solver
void
KissatSolver::randomDiversify(int id)
{
//    solver->randomVarsActivitiesAndPolarities(id);
}

// Solve the formula with a given set of assumptions
// return 10 for SAT, 20 for UNSAT, 0 for UNKNOWN
SatResult
KissatSolver::solve(const vector<int> & cube)
{
   unsetSolverInterrupt();

   vector<ClauseExchange *> tmp;

   tmp.clear();
   clausesToAdd.getClauses(tmp);

   for (size_t ind = 0; ind < tmp.size(); ind++) {

       vector<int> cls;
       for(int i=0; i< tmp[ind]->size; i++) cls.push_back(tmp[ind]->lits[i]);
      ClauseManager::releaseClause(tmp[ind]);


      for( int i=0; i< cls.size(); i++ ) kissat_add(solver, cls[i]);//solver->add(cls[i]);
      kissat_add(solver, 0); 
   }
   
   int res = kissat_solve(solver);

   if(res == 10 || res == 20) {
       kissat_print_statistics(solver);
       printf("\nc time = %f *** ##### Solver id: %d / answer = %s\n",getRelativeTime(), id, res == 10 ? "SAT" : "UNSAT");
       fflush(stdout);
   }
   
   if (res == 10)
      return SatResult::SAT;

   if (res == 20)
      return SatResult::UNSAT;

   return SatResult::UNKNOWN;

}

void
KissatSolver::addClause(ClauseExchange * clause)
{
   clausesToAdd.addClause(clause);

   setSolverInterrupt();
}

void
KissatSolver::addLearnedClause(ClauseExchange * clause)
{
   if (clause->size == 1) {
      unitsToImport.addClause(clause);
   } else {
      clausesToImport.addClause(clause);
   }
}

void
KissatSolver::addClauses(const vector<ClauseExchange *> & clauses)
{
   clausesToAdd.addClauses(clauses);

   setSolverInterrupt();
}

void
KissatSolver::addInitialClauses(const vector<ClauseExchange *> & clauses)
{
   for (size_t ind = 0; ind < clauses.size(); ind++) {

      for (size_t i = 0; i < clauses[ind]->size; i++) {
         int lit = clauses[ind]->lits[i];
         kissat_add(solver, lit); 
      }
      kissat_add(solver, 0);

   }
}

void
KissatSolver::addLearnedClauses(const vector<ClauseExchange *> & clauses)
{
   for (size_t i = 0; i < clauses.size(); i++) {
      addLearnedClause(clauses[i]);
   }
}

void
KissatSolver::getLearnedClauses(vector<ClauseExchange *> & clauses)
{
   clausesToExport.getClauses(clauses);
}

void
KissatSolver::increaseClauseProduction()
{
   lbdLimit++;
}

void
KissatSolver::decreaseClauseProduction()
{
   if (lbdLimit > 2) {
      lbdLimit--;
   }
}

SolvingStatistics
KissatSolver::getStatistics()
{
   SolvingStatistics stats;
//   CaDiCaL::Stats& st = solver->getStats();
//   stats.conflicts    = st.conflicts;
//   stats.propagations = st.propagations.search;
//   stats.restarts     = st.restarts;
//   stats.decisions    = st.decisions ;
//   stats.nbImportedClauses = st.nbImportedClauses;
//   stats.nbImportedUnits = st.nbImportedUnits;
//   stats.nbExportedClauses = st.nbExportedClauses;
//   stats.nbExportedUnits = st.nbExportedUnits;

   return stats;
}

std::vector<int>
KissatSolver::getModel()
{
   std::vector<int> model;

   for (unsigned i = 1; i <= getVariablesCount(); i++) {
       int lit = kissat_value(solver, i);
       if(lit == 0) lit = i;
       model.push_back(lit);
   }

   return model;
}
