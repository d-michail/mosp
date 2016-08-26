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
#include <LEDA/system/assert.h>
#include <LEDA/graph/mwb_matching.h>
#else
#include <LEDA/graph.h>
#include <LEDA/std/assert.h>
#include <LEDA/mwb_matching.t>
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
    using leda::node_map;
    using leda::edge;
    using leda::edge_array;
    using leda::edge_map;
#endif

    /*  \brief Compute the unpopularity margin of a matching. See McCutchen 2007.
     * 
     *  The edge order might change during the execution of this procedure.
     *
     *  \param G A bipartite graph.
     *  \param A The list of nodes on the left side of the bipartite graph.
     *  \param B The list of nodes on the right side of the bipartite graph.
     *  \param rank An edge array. Each entry corresponds to the rank of an edge.
     *  \param M The matching to test in the following form. For each vertex the edge that it is 
     *           matched or nil if it is free.
     *  \pre   All edges are directed from A to B.
     *  \return The unpopularity margin.
     */
    int BI_UNPOPULARITY_MARGIN( graph& G, 
            const list<node>& A,  
            const list<node>& B,
            const edge_array<int>& rank,
            const node_array<edge>& M
            )
    {
        if ( ! G.is_directed() )
            leda::error_handler(999, "BI_UNPOPULARITY_FACTOR: graph should be directed!");

        // make new weight function
        edge_array<int> weight( G, 0 );
        
        node v;
        edge e, em;
        int re, rem;
        forall( v, A ) 
        {
            if ( M[v] == nil )  // is free
                leda::error_handler(999, "Node should not be free, we have last resorts!");

            // add edges
            em = M[v];
            rem = rank[em];
            if ( v != G.source(em) )
                leda::error_handler(999, "UNPOPULAR_MARGIN: edges are not directed from applicants to posts!");

            forall_adj_edges( e, v ) 
            {
                re = rank[e];

                if ( re < rem ) 
                    weight[e] = 1;
                else if ( re == rem )
                    weight[e] = 0;
                else 
                    weight[e] = -1;
            }
        }

        // now compute a maximum weight matching among the 
        // matchings with maximum cardinality
        //
        // (implementation note: const problem here since LEDA does not use const)
        list<edge> mwm = MWMCB_MATCHING( G, A, B, weight );

        int margin = 0;
        forall( e, mwm ) { 
            margin += weight[e];
        }

        return margin;
    }

    /*  \brief Compute the unpopularity margin of a matching. See McCutchen 2007.
     *
     *  The edge order might change during the execution of this procedure.
     *
     *  \param G A bipartite graph.
     *  \param A The list of nodes on the left side of the bipartite graph.
     *  \param B The list of nodes on the right side of the bipartite graph.
     *  \param rank An edge array. Each entry corresponds to the rank of an edge.
     *  \param M The matching to test as a list of edges.
     *  \pre   All edges are directed from A to B.
     *  \return The unpopularity margin
     */
    int BI_UNPOPULARITY_MARGIN( graph& G, 
            const list<node>& A,  
            const list<node>& B,
            const edge_array<int>& rank,
            const list<edge>& M
            )
    {
        node v;
        edge e;

        // add last resorts
        list<node> Bls; 
        forall( v, B ) 
            Bls.push( v );

        edge_map<int> newrank( G );
        node_map<edge> lastResort( G );

        forall( v, A ) {
            int maxrank = 0;
            forall_adj_edges( e, v ) { 
                if ( rank[e] > maxrank ) 
                    maxrank = rank[e];
                newrank[e] = rank[e];
            }
            e = G.new_edge( v, G.new_node() );
            lastResort[ v ] = e;
            Bls.push( G.target(e) );
            newrank[e] = maxrank+1;
        }

        // compute margin
        node_array<edge> Me(G, nil);
        forall( e, M ) 
        {   
            Me[ G.source(e) ] = e;
            Me[ G.target(e) ] = e;
        }

        forall( v, A ) 
        {
            if ( Me[v] == nil ) 
            { 
                e = lastResort[v];
                Me[v] = e;
                Me[ G.opposite( v, e ) ] = e;
            }
        }

        int margin = BI_UNPOPULARITY_MARGIN( G, A, Bls, newrank, Me );

        // remove last resorts
        forall( v, A )
            G.del_node( G.opposite( lastResort[v], v ) );

        return margin;
    }


} // end of namespace

/* ex: set ts=4 sw=4 sts=4 et: */

