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
#include <LEP/mosp/graphcopy.h>
#include <LEP/mosp/util.h>

#ifdef LEDA_GE_V5
#include <LEDA/core/stack.h>
#include <LEDA/core/queue.h>
#include <LEDA/core/array.h>
#include <LEDA/graph/graph.h>
#else
#include <LEDA/stack.h>
#include <LEDA/queue.h>
#include <LEDA/array.h>
#include <LEDA/graph.h>
#endif


namespace mosp
{

#if defined(LEDA_NAMESPACE)
    using leda::graph;
    using leda::list;
    using leda::list_item;
    using leda::stack;
    using leda::array;
    using leda::queue;
    using leda::node;
    using leda::node_list;
    using leda::node_array;
    using leda::edge;
    using leda::edge_array;
#endif


    class RankMaximalCapacitatedMatching
    {   
        public:
            RankMaximalCapacitatedMatching( const graph& G_, 
                             const list<node>& A_,
                             const list<node>& B_,
                             const node_array<int>& capacity_,
                             const edge_array<int>& rank_
                            ) 
                : G(G_), rankSetSize( 10 ), number_of_augmentations(0)
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

                G.copy_node_list( A_, A );
                G.copy_node_list( B_, B );
                G.copy_node_array( capacity_, capacity );
                G.copy_edge_array( rank_, rank );
            }

            void run( list<edge>& L ) { 
                list<edge> q;

                RANK_MAX_CAPACITATED_PRIV( q );

                L.clear();
                edge e;
                forall( e, q )
                    L.append( G.map( e ) );
            }

        private:

            void FindMinAndMaxRank() 
            { 
                min_rank = MAXINT; 
                max_rank = 1;
                edge e;
                forall_edges( e, G ) {
#if ! defined(LEDA_CHECKING_OFF)
                    if( rank[e] <= 0 ) 
                        leda::error_handler(999, "RANK MAXIMAL CAPACITATED: illegal rank (non-positive)");
#endif
                    if ( rank[e] > max_rank ) max_rank = rank[e];
                    if ( rank[e] < min_rank ) min_rank = rank[e];
                }
            }

            void CountRankSizes()
            {
                edge e;
                rankSetSize.resize( max_rank + 1 );
                rankSetSize.init( 0 );
                forall_edges( e, G ) 
                    rankSetSize[ rank[e] ]++;
            }

            void CheckValidCapacities()
            {
                node v;
                forall( v, A ) { 
                    if ( capacity[v] < 0 || capacity[v] > 1 ) 
                        leda::error_handler(999, "RANK MAXIMAL CAPACITATED: illegal left side capacity");
                }
                forall( v, B ) { 
                    if ( capacity[v] < 0 )
                        leda::error_handler(999, "RANK MAXIMAL CAPACITATED: illegal right side capacity");
                }
            }

            void DirectEdgesFromAtoB() {
                node v;
                edge e;
                forall( v, A ) {
                    forall_in_edges( e, v ) 
                        G.rev_edge( e );
                }
            }

            void SortGraphEdgesByRank() { 
                G.sort_edges( rank );
            }

            bool free( node v ) { 
                return capacity[v] > 0;
            }

            bool find_aug_path_by_bfs( 
                    node a, 
                    node_array<edge>& pred, 
                    node_array<int>&  mark
                    )
            {
                //std::cout << "Looking for augmenting path out of " << a->id() << std::endl;
                //std::cout << "augmentation number = " << number_of_augmentations << std::endl;
                queue<node> Q;
                Q.append( a );
                mark[a] = number_of_augmentations;

                edge e, f;
                while( !Q.empty() )
                {
                    node v = Q.pop();    // v is a node in A
                    //std::cout << "current node is " << v->id() << std::endl;
            
                    forall_adj_edges( e, v ) 
                    {
                        node w = G.target(e);   // w is a node in B
                        //std::cout << "now node " << w->id() << " (in B)" << std::endl;
                        //std::cout << "mark of node " << w->id() << " is " << mark[w] << std::endl;
                        if ( mark[w] == number_of_augmentations ) 
                            continue;

                        //std::cout << "node " << w->id() << " is not reached" << std::endl;
                        // w has not been reached in this search
                        pred[w] = e; 
                        mark[w] = number_of_augmentations;
                        //std::cout << "node " << w->id() << " is now marked as " << mark[w] << std::endl;

                        if ( free(w) ) { // augment from a to w
                            //std::cout << "node " << w->id() << " is free, augmenting" << std::endl;
                            capacity[w]--; 
                            capacity[a]--;
                            while( w != a ) { 
                                e = pred[w];
                                w = G.source(e);
                                //std::cout << "Reversing edge (" << w->id() << "," << G.target(e)->id() << ")" << std::endl;
                                G.rev_edge(e);
                            }
                            return true;
                        }

                        // DEBUG
                        //std::cout << "node " << w->id() << " not free" << std::endl;

                        // w is not free
                        forall_adj_edges( f, w ) 
                        {
                            node x = G.target(f);
                            pred[x] = f; 
                            mark[x] = number_of_augmentations;
                            //std::cout << "Appending node " << x->id() << " to be visited" << std::endl;
                            Q.append(x);
                        }
                    }
                }
                return false;
            }

            // typedef for bfs
            struct bfs_info { 
                bool odd; 
                node n; 

                bfs_info(): odd(false), n(nil) {}
            };

            void partition_graph_node_set_by_bfs( 
                    node_array<bool>& reached,
                    node_array<bool>& odd,
                    stack<node>& reached_nodes
                    )
            {
                //std::cout << "partitioning graph node set by bfs" << std::endl;
                list<bfs_info> Q;
                node v; edge e;

                forall(v, A ) { // add initial nodes
                    if ( capacity[v] <= 0 ) continue;
                    bfs_info i; 
                    i.n = v; 
                    i.odd = false;
                    Q.append( i );
                    //std::cout << "node " << v->id() << " is free" << std::endl;
                }

                while( ! Q.empty() ) { // now traverse in bfs style
                    bfs_info i = Q.pop();

                    reached[ i.n ] = true; 
                    odd[ i.n ] = i.odd;
                    reached_nodes.push( i.n );

                    //std::cout << "node " << i.n->id() << " is " << ((i.odd)?"odd":"even") << std::endl;

                    forall_adj_edges( e , i.n ) {   

                        //std::cout << "following edge (" << G.source(e)->id() << "," << G.target(e)->id() << ")" << std::endl;

                        if ( !reached[ G.opposite(e, i.n) ] ) { 
                            bfs_info nexti; 
                            nexti.odd = ! i.odd;
                            nexti.n = G.opposite(e, i.n);
                            Q.append( nexti );
                        }
                    }
                }
            }


            // remove edges that are present in the current
            // graph which will never be used in a maximum matching
            void prune_half_edges( const node_array<bool>& reached,
                                   const node_array<bool>& odd 
                                 )
            {
                //std::cout << "pruning already existing edges" << std::endl;
                edge e; 
                node v, w;
                forall( v, A ) 
                {
                    if ( reached[v] ) 
                        continue;

                    forall_out_edges( e, v )  // ?? inout, out or adj
                    {
                        w = G.opposite( e , v );

                        // !! odd(e) is not well defined if reached(e) == false !!
                        bool w_is_odd = reached[ w ] & odd[ w ];

                        if ( w_is_odd )  { 
                            //std::cout << "pruning edge (" << G.source(e)->id() << "," << G.target(e)->id() << ")" << std::endl;
                            G.del_edge(e);
                        }
                    }
                }
            }


            // main function to compute a capacitated rank maximal matching
            void RANK_MAX_CAPACITATED_PRIV( list<edge>& L )
            {
                CheckValidCapacities();
                FindMinAndMaxRank();
                CountRankSizes();
                DirectEdgesFromAtoB();
                SortGraphEdgesByRank();

                // record all edges in specified order and hide them from the graph
                node v, u; 
                edge e;
                node_array< list<edge> > edgesA( G );
                forall( v, A ) {
                    edgesA[ v ] = list<edge>();
                    forall_out_edges( e , v )
                        edgesA[v].append( e );
                    forall_out_edges( e, v )
                        G.hide_edge( e );
                }

                // now define some stuff relevant to the matching
                node_array<bool> reached(G, false);
                node_array<bool> odd(G);
                stack<node> reached_nodes;

                // and stuff relevant to the augmentation algorithm
                node_array<edge> pred(G);
                node_array<int> mark(G, -1);

                // main loop
                int phase = min_rank;
                while( phase <= max_rank ) 
                {
                    // check that we have new edges in this phase
                    if ( rankSetSize[ phase ] == 0 ) { 
                        phase++;
                        continue;
                    }

                    //std::cout << "Starting phase " << phase << std::endl;

                    // find decomposition of nodes
                    while( !reached_nodes.empty() ) { 
                        reached[ reached_nodes.top() ] = false; 
                        reached_nodes.pop(); 
                    }
                    partition_graph_node_set_by_bfs( reached, odd, reached_nodes );
    
                    // prune edges already added
                    prune_half_edges( reached, odd ); 

                    // prune edges that will be added
                    //std::cout << "Pruning larger rank edges" << std::endl;
                    forall( v, A ) {
                        list_item it, itnext;
                        it = edgesA[v].first();
                        while( it != NULL ) 
                        {
                            e = edgesA[v].inf(it);
                            u = G.opposite(e, v);
                            
                            itnext = edgesA[v].succ(it);
                            if ( !reached[v] || ( reached[u] && odd[u] ) ) { 
                                rankSetSize[ rank[e] ]--;
                                edgesA[v].del_item( it );
                                //std::cout << "pruning edge (" << G.source(e)->id() << "," << G.target(e)->id() << ")" << std::endl;
                                G.restore_edge(e);
                                G.del_edge(e);
                            }
                            it = itnext;
                        }
                    }

                    // add current phase edges that survived the pruning
                    //std::cout << "Adding current phase edges that survived the pruning" << std::endl;
                    forall( v, A ) { 
                        //std::cout << "checking for new edges out of node " << G[v] << std::endl;

                        list_item it;
                        while( ! edgesA[v].empty() )
                        {   
                            it = edgesA[v].first();
                            e = edgesA[v].inf( it );
                            if ( rank[e] == phase )
                            { 
                                it = edgesA[v].succ( it );
                                edgesA[v].pop_front();
                                G.restore_edge( e );
                                //std::cout << "restoring edge (" << G.source(e)->id() << "," << G.target(e)->id() << ")" << std::endl;
                                rankSetSize[ rank[e] ]--;
                            }
                            else 
                                break;
                        }
                    }

                    // find max matching by augmentations
                    number_of_augmentations++;  // this is very important
                    forall(v, A ) { 
                        if ( !free(v) ) 
                            continue;
                        if ( find_aug_path_by_bfs( v, pred, mark ) )
                            number_of_augmentations++;
                    }
            
                    // increase phase
                    phase++;
                    //std::cout << "Increasing phase number" << std::endl;
                    //std::cout << "*******************************************************" << std::endl;
                }

                // now return matching
                forall( v, A ) 
                    forall_in_edges( e, v )
                        L.append( e );
            }

        private:
                
            // graph
            graph_copy G;
            list<node> A, B;
            edge_map<int> rank;
            node_map<int> capacity;

            // rank related
            int min_rank, max_rank;    
            array<int> rankSetSize;
            int totalEdgeSetSize;

            // augmentation related
            int number_of_augmentations;
    };

    list<edge> BI_RANK_MAX_CAPACITATED_MATCHING( 
            const graph& G, 
            const list<node>& A, 
            const list<node>& B, 
            const node_array<int>& capacity,
            const edge_array<int>& rank 
            )
    {
        list<edge> L;
        RankMaximalCapacitatedMatching pm( G, A, B, capacity, rank );
        pm.run( L );
        return L;
    }

    // debugging
    bool DEBUG_is_valid_matching( const graph &G, 
            const node_array<int>& capacity,
            const list<edge>& matching ) { 
        node_array<int> count(G,0);
        edge e;
        node v,u;
        forall (e, matching) { 
            v = G.source(e);
            u = G.target(e);
            count[v]++;
            count[u]++;
            if ( count[v] > capacity[v] ) return false;
            if ( count[u] > capacity[u] ) return false;
        }
        return true;
    }



} // end of namespace

/* ex: set ts=4 sw=4 sts=4 et: */

