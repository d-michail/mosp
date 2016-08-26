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

#ifdef LEDA_GE_V5
#include <LEDA/graph/graph.h>
#else
#include <LEDA/graph.h>
#endif


namespace mosp
{

#if defined(LEDA_NAMESPACE)
    using leda::graph;
    using leda::list;
    using leda::list_item;
    using leda::node;
    using leda::node_list;
    using leda::node_array;
    using leda::edge;
    using leda::edge_array;
#endif

    /*  Check that a bipartite partition is valid
     *  \param G The input graph
     *  \param A The nodes on the left side.
     *  \param B The nodes on the right side.
     *  \return True if the give node partition bipartite and false otherwise.
     */
    bool check_bipartite( const graph& G, const list<node>& A, const list<node>&B )
    {
        node_array<bool> isinA ( G, false );
        node v;
        forall(v,A) isinA[v]=true;
        edge e;
        bool l,r;
        forall_edges( e, G ) { 
            l = isinA[ G.source(e) ];
            r = isinA[ G.target(e) ];
            if ( (l && r) || (!l && !r) ) return false;
        }
        return true;
    }

} // end of namespace

/* ex: set ts=4 sw=4 sts=4 et: */

