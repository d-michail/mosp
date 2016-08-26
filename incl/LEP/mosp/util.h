//
// This program can be freely used in an academic environment
// ONLY for research purposes, subject to the following restrictions:
//
// 1. The origin of this software must not be misrepresented; you must not
//    claim that you wrote the original software. If you use this software
//    an acknowledgment in the product documentation is required.
// 2. Altered source versions must be plainly marked as such, and must not be
//    misrepresented as being the original software.
// 3. This notice may not be removed or altered from any source distribution.
//
// Any other use is strictly prohibited by the author, without an explicit
// permission.
//
// Note that this program uses the LEDA library, which is NOT free. For more
// details visit Algorithmic Solutions at http://www.algorithmic-solutions.com/
//
// !!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!
// ! Any commercial use of this software is strictly !
// ! prohibited without explicit permission by the   !
// ! author.                                         !
// !!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!
//
// This software is distributed in the hope that it will be useful,
// but WITHOUT ANY WARRANTY; without even the implied warranty of
// MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.
//
// Copyright (C) 2004-2010 Dimitrios Michail <dimitrios.michail@gmail.com>

/*! \file util.h
 *  \brief Various helper functions
 */

#ifndef LEP_UTIL_H
#define LEP_UTIL_H

#include <LEP/mosp/config.h>

#ifdef LEDA_GE_V5
#include <LEDA/graph/graph.h>
#include <LEDA/core/list.h>
#include <LEDA/system/assert.h>
#else
#include <LEDA/graph.h>
#include <LEDA/list.h>
#include <LEDA/std/assert.h>
#endif

namespace mosp
{

    /** Check that a bipartite partition is valid
     *  \param G The input graph
     *  \param A The nodes on the left side.
     *  \param B The nodes on the right side.
     *  \return True if the give node partition bipartite and false otherwise.
     */
    bool check_bipartite( const leda::graph& G, 
                          const leda::list<leda::node>& A, 
                          const leda::list<leda::node>&B 
                        );

}

#endif  // LEP_POPULAR_H

/* ex: set ts=4 sw=4 sts=4 et: */


