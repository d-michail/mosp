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
#include "popular_clone.h"

#ifdef LEDA_GE_V5
#include <LEDA/core/list.h>
#include <LEDA/graph/graph.h>
#include <LEDA/graph/node_map.h>
#else
#include <LEDA/list.h>
#include <LEDA/graph.h>
#include <LEDA/node_map.h>
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

            CapacityCloner::CapacityCloner( const graph& G_, 
                            const list<node>& A_,
                            const list<node>& B_,
                            const edge_array<int>& rank_,
                            const node_array<int>& capacity_
                            ) 
                : sourceG(G_), sourceA(A_), sourceB(B_), sourcerank(rank_), sourcecapacity(capacity_)
            { 
#if ! defined(LEDA_CHECKING_OFF)
#if __LEDA__ >= 600
                if ( ! G_.is_directed() )
                    leda::error_handler(999, "BI_RANK_MAX: The input graph needs to be directed!");
#endif
                if ( Is_Undirected_Simple( G_ ) == false )
                    leda::error_handler(999, "POPULAR: illegal graph (non-simple)" );
                if ( Is_Loopfree( G_ ) == false )
                    leda::error_handler(999, "POPULAR: illegal graph (has loops)" );
                if ( check_bipartite( G_, A_, B_ ) == false )
                    leda::error_handler(999, "POPULAR: illegal partition (non-bipartite)" );
#endif
                init();
            }

            graph& CapacityCloner::getClonedGraph() { return G; }

            list<node>& CapacityCloner::getClonedPartitionA() { return A; }

            list<node>& CapacityCloner::getClonedPartitionB() { return B; }

            edge_array<int>& CapacityCloner::getClonedRank() { return rank; }

            edge CapacityCloner::clonedToOriginal( edge e ) 
            {
                assert( e != nil );
                return emap[e];
            }

            void CapacityCloner::clonedToOriginal( const list<edge>& cloned, list<edge>& original )
            {
                edge e;
                original.clear();
                forall( e, cloned ) 
                    original.append( emap[e] );
            }

    
            void CapacityCloner::init()
            {
                node v, u;

                // initialize node mapping
                leda::node_map< leda::list< leda::node > > clones( sourceG );
                forall_nodes(v, sourceG)
                    clones[v] = leda::list< leda::node > ();

                // initialize edge mapping
                emap.init( G );

                // create nodes in A
                forall( v, sourceA ) { 
                    assert( sourcecapacity[v] == 1 );
                    u = G.new_node();
                    A.append(u);
                    clones[v].append( u );
                }

                // create nodes in B
                forall( v, sourceB ) { 
                    assert( sourcecapacity[v] >= 1 );
                    for( int i = 0; i < sourcecapacity[v]; i++ ) {
                        u = G.new_node(); 
                        B.append( u );
                        clones[v].append( u );
                    }
                }

                // create edges and give ranks
                edge e, e1;
                bool forward;
                list_item it;
                forall( v, sourceA ) 
                {
                    forall_inout_edges( e, v ) 
                    {
                        forward = ( v == sourceG.source(e) );
                        u = sourceG.opposite( e, v );

                        for( it=clones[u].first(); it!=NULL; it=clones[u].succ(it) )
                        {
                            if ( forward )
                                e1 = G.new_edge( v, clones[u].inf( it ) );
                            else 
                                e1 = G.new_edge( clones[u].inf( it ), v );

                            emap[e1] = e; // record backward mapping
                        }
                    }
                }

                // fix ranks
                rank.init( G );
                forall_edges( e, G ) 
                    rank[e] = sourcerank[ emap[e] ];
            }

} // end of namespace

/* ex: set ts=4 sw=4 sts=4 et: */

