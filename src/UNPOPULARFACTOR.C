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
#include <LEDA/graph/templates/shortest_path.h>
#else
#include <LEDA/graph.h>
#include <LEDA/std/assert.h>
#include <LEDA/templates/shortest_path.t>
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

    /*  \brief Compute the unpopularity factor of a matching. The unpopularity factor is 
     *  based on the definition with the multiplicities. See McCutchen 2007.
     *
     *  \param G A bipartite graph.
     *  \param A The list of nodes on the left side of the bipartite graph.
     *  \param B The list of nodes on the right side of the bipartite graph.
     *  \param rank An edge array. Each entry corresponds to the rank of an edge.
     *  \param M The matching to test in the following form. For each vertex the edge that it is 
     *           matched or nil if it is free.
     *  \param factor Contains the unpopularity factor if it is finite. Undefined otherwise.
     *  \pre   All edges are directed from A to B.
     *  \pre   The adjacency lists of the graph must be sorted according to the rank parameter. This
     *         means that in LEDA someone has to call G.sort_edges( rank ) before calling this function.
     *  \return True if the matching has a finite unpopularity factor, false if infinite.
     */
    bool BI_UNPOPULARITY_FACTOR( const graph& G, 
            const list<node>& A,  
            const list<node>& B,
            const edge_array<int>& rank,
            const node_array<edge>& M,
            int &factor )
    {
        if ( ! G.is_directed() )
            leda::error_handler(999, "BI_UNPOPULARITY_FACTOR: graph should be directed!");

        // record free vertices
        node v,u;
        //list<node> free;
        //forall( v, A )
        //    if ( M[v] == nil ) 
        //        free.append(v);

        // make new graph
        graph H;
        //edge_array<int> weight( H, G.number_of_edges() + B.size() + 2 * free.size() , 0 );
        edge_array<int> weight( H, G.number_of_edges() + B.size(), 0 );
        node_array< node > map ( G, nil );
        forall( v, B )  
            map[ v ] = H.new_node();
        //forall( v, free ) 
        //    map[ v ] = H.new_node();
        node s = H.new_node(); // make source

        // add edges
        edge e, em;
        forall( v, A ) { 
            if ( M[v] == nil ) { // is free
                //forall_adj_edges( e, v ) { 
                //    weight[ H.new_edge( map[v], map[ G.opposite(e,v) ] ) ] = -1;
                //}
                continue;
            }

            em = M[v]; 
            if ( v != G.source(em) )
                leda::error_handler(999, "POPULAR: edges are not directed from applicants to posts!");
            u = G.opposite( v, em );

            e = G.adj_pred( em ); 
            while( e != nil ) { 
                if ( rank[e] == rank[em] ) {
                    //std::cout << "weight = 0" << std::endl;
                    weight[ H.new_edge( map[u], map[G.opposite(e,v)] ) ] = 0;
                }
                else if ( rank[e] < rank[em] ) { 
                    //std::cout << "weight = -1" << std::endl;
                    weight[ H.new_edge( map[u], map[G.opposite(e,v)] ) ] = -1;
                }
                else 	
                    leda::error_handler(999, "POPULAR: graph edges are not sorted by rank!");

                e = G.adj_pred( e );
            }
        }

        // add edges from sink to all others
        forall(v, B) 
            weight[ H.new_edge( s, map[v] ) ] = 0;
        //forall(v, free)
        //    weight[ H.new_edge( s, map[v] ) ] = 0;

        // run Bellman-Ford
        node_array<int> dist( H );
        node_array<edge> pred( H );
        bool success = BELLMAN_FORD_T( H, s, weight, dist, pred );

        // if negative cycle return INFINITE unpopularity factor
        if ( success == false ) 
            return false;

        // check if negative length path to unfilled position
        int min = 0;
        forall( v, B ) { 
            if ( M[v] == nil && dist[ map[v] ] < 0 ) 
                return false;
            if ( dist[ map[v] ] < min ) 
                min = dist[ map[v] ];
        }

        // else compute maximum unpopularity factor
        //int min = 0;
        //forall_nodes( v, H ) {
            //std::cout << "distance " << dist[v] << std::endl;
            //if ( dist[v] < min ) 
                //min = dist[v];
        //}

        // return it
        factor = -min;
        return true;
    }

    /*  \brief Compute the unpopularity factor of a matching. The unpopularity factor is 
     *  based on the definition with the multiplicities. See McCutchen 2007.
     *
     *  \param G A bipartite graph.
     *  \param A The list of nodes on the left side of the bipartite graph.
     *  \param B The list of nodes on the right side of the bipartite graph.
     *  \param rank An edge array. Each entry corresponds to the rank of an edge.
     *  \param M The matching to test in the following form. A list of edges.
     *  \param factor Contains the unpopularity factor if it is finite. Undefined otherwise.
     *  \pre   All edges are directed from A to B.
     *  \pre   The adjacency lists of the graph must be sorted according to the rank parameter. This
     *         means that in LEDA someone has to call G.sort_edges( rank ) before calling this function.
     *  \return True if the matching has a finite unpopularity factor, false if infinite.
     */
    bool BI_UNPOPULARITY_FACTOR( graph& G, 
            const list<node>& A,  
            const list<node>& B,
            const edge_array<int>& rank,
            const list<edge>& M,
            int &factor )
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

        // compute factor 
        node_array<edge> Me(G, nil);
        forall( e, M ) 
        {   
            Me[ G.source(e) ] = e;
            Me[ G.target(e) ] = e;
        }

        forall( v, A ) 
        {
            if ( Me[ v ] == nil ) {
                e = lastResort[v];
                Me[ v ] = e;
                Me[ G.opposite( v, e ) ] = e;
            }
        }

        bool ret = BI_UNPOPULARITY_FACTOR( G, A, Bls, newrank, Me, factor );

        // remove last resorts
        forall( v, A )
            G.del_node( G.opposite( lastResort[v], v ) );

        return ret;
    }


} // end of namespace

/* ex: set ts=4 sw=4 sts=4 et: */

