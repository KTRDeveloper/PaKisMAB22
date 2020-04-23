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

#include "sharing/Sharer.h"
#include "solvers/SolverInterface.h"
#include "working/WorkingStrategy.h"

#include <atomic>
#include <vector>

using namespace std;

/// Is it the end of the search
extern atomic<bool> globalEnding;

/// Working strategy
extern WorkingStrategy * working;

/// Array of sharers
extern Sharer ** sharers;

/// Size of the array of sharers
extern int nSharers;

/// Final result
extern SatResult finalResult;

/// Model for SAT instances
extern vector<int> finalModel;
