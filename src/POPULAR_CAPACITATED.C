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

#include <LEP/mosp/config.h>
#include <LEP/mosp/util.h>
#include <LEP/mosp/POPULAR.h>
#include "popular_clone.h"

#ifdef LEDA_GE_V5
#include <LEDA/core/stack.h>
#include <LEDA/graph/graph.h>
#else
#include <LEDA/stack.h>
#include <LEDA/graph.h>
#endif


namespace mosp
{

#if defined(LEDA_NAMESPACE)
    using leda::graph;
    using leda::list;
    using leda::list_item;
    using leda::stack;
    using leda::node;
    using leda::node_list;
    using leda::node_array;
    using leda::edge;
    using leda::edge_array;
#endif

    bool BI_POPULAR_CAPACITATED_MATCHING( 
            const graph& G, 
            const list<node>& A, 
            const list<node>& B, 
            const edge_array<int>& rank, 
            const node_array<int>& capacity,
            list<edge>& L )
    {
        CapacityCloner cloner( G, A, B, rank, capacity );

        list<edge> clonedL;
        bool exists = BI_POPULAR_MATCHING( cloner.getClonedGraph(), 
                                           cloner.getClonedPartitionA(), 
                                           cloner.getClonedPartitionB(),
                                           cloner.getClonedRank(), 
                                           clonedL
                                           );

        if ( exists )
            cloner.clonedToOriginal( clonedL, L );

        return exists;
    }

} // end of namespace

/* ex: set ts=4 sw=4 sts=4 et: */

