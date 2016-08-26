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
//

#ifndef LEP_POPULAR_CLONE_H
#define LEP_POPULAR_CLONE_H

#include <LEP/mosp/config.h>

#ifdef LEDA_GE_V5
#include <LEDA/graph/graph.h>
#include <LEDA/system/assert.h>
#include <LEDA/core/list.h>
#include <LEDA/core/array.h>
#else
#include <LEDA/graph.h>
#include <LEDA/std/assert.h>
#include <LEDA/list.h>
#include <LEDA/array.h>
#endif

namespace mosp
{

    class CapacityCloner
    {   
        public:
            CapacityCloner( const leda::graph& G_, 
                            const leda::list<leda::node>& A_,
                            const leda::list<leda::node>& B_,
                            const leda::edge_array<int>& rank_,
                            const leda::node_array<int>& capacity_
                            ); 

            leda::graph& getClonedGraph();

            leda::list<leda::node>& getClonedPartitionA();

            leda::list<leda::node>& getClonedPartitionB();

            leda::edge_array<int>& getClonedRank();

            leda::edge clonedToOriginal( leda::edge e );

            void clonedToOriginal( const leda::list<leda::edge>& cloned, leda::list<leda::edge>& original );

        private:

            void init();

        private:
                
            // references to original graph
            const leda::graph& sourceG; 
            const leda::list<leda::node>& sourceA;
            const leda::list<leda::node>& sourceB;
            const leda::edge_array<int>& sourcerank;
            const leda::node_array<int>& sourcecapacity;

            // cloned graph
            leda::graph G;
            leda::list<leda::node> A, B;
            leda::edge_array<int> rank;

            // edge backward mapping
            leda::edge_map<leda::edge> emap;
    };

} // end of namespace

#endif

/* ex: set ts=4 sw=4 sts=4 et: */

