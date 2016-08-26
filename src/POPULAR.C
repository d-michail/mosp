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

    // such a helper
#define is_matched(edge) ( mate[source(edge)] == target(edge) )
#define is_node_matched(node) ( mate[node] != nil )

    // typedef for bfs
    struct bfs_info { 
        bool odd, follow_both, follow_free; node n; 

        bfs_info(): odd(false), follow_both(false), follow_free(false), n(nil) {}
    };

    static void partition_graph_node_set_by_bfs( graph& G,
            const list<node>& A,
            const list<node>& B,
            const node_array<bool>& free,
            const node_array<node>& mate,
            node_array<bool>& reached,
            node_array<bool>& odd,
            stack<node>& reached_nodes
            )
    {
        list<bfs_info> Q;
        node v; edge e;

        forall_nodes(v, G ) { // add initial nodes
            if ( ! free[v] ) continue;
            bfs_info i; i.n = v; i.follow_both = true; 
            i.odd = false; i.follow_free = true;
            Q.append( i );
        }

        while( ! Q.empty() ) { // now traverse in bfs style
            bfs_info i = Q.pop();
            reached[ i.n ] = true; odd[ i.n ] = i.odd;
            reached_nodes.push( i.n );

            forall_inout_edges( e , i.n ) { 

                if ( ( i.follow_both || i.follow_free ) && 
                        !reached[ G.opposite(e,i.n) ] && !is_matched(e) ) { 
                    bfs_info nexti; nexti.odd = ! i.odd; nexti.n = G.opposite(e, i.n);
                    nexti.follow_both = false; nexti.follow_free = false;
                    Q.append( nexti );
                }
                if( ( i.follow_both || ! i.follow_free ) && 
                        !reached[ G.opposite(e,i.n) ] && is_matched(e) ) {
                    bfs_info nexti; nexti.odd = ! i.odd; nexti.n = G.opposite( e, i.n );
                    nexti.follow_both = false; nexti.follow_free = true;
                    Q.append( nexti );
                }
            }
        }
    }

    static void prune_edges( graph& G, 
            const list<node>& A,
            const list<node>& B,
            node_array<bool>& reached,
            const node_array<bool>& odd )
    {
        edge e; node v, w;
        bool v_is_odd, w_is_odd;
        forall( v, A ) 
        {
            forall_inout_edges( e, v ) 
            {
                w = G.opposite( e , v );
                // !! odd(e) is not well defined if reached(e) == false !!
                // TODO: should this be && or & ??
                v_is_odd = reached[ v ] & odd[ v ];
                w_is_odd = reached[ w ] & odd[ w ];

                if ( (v_is_odd && ( !reached[ w ] || w_is_odd )) || 
                        (w_is_odd && ( !reached[ v ] || v_is_odd )) ) {
                    //std::cout << "pruned edge " << G[e] << std::endl;
                    G.del_edge(e);
                }
            }
        }
    }

    static int phase_number_HK;

    // an directed edge on an undirected graph
    typedef struct { edge e; node s,t; } d_edge;

    static bool bfs_HK(graph& G, 
            const node_list& free_in_A, 
            const node_array<bool>& free, 
            node_array<node>& mate,
            edge_array<int>& useful,
            node_array<int>& dist, 
            node_array<int>& reached_HK )
    {
        list<bfs_info> Q;
        node v,w;
        edge e;

        forall(v, free_in_A) { 
            bfs_info f; f.n = v; f.follow_free = true;
            Q.append(f);
            dist[v] = 0;
            reached_HK[v] = phase_number_HK;
        }
        bool augmenting_path_found = false;

        while( ! Q.empty() ) { 
            bfs_info f = Q.pop();
            v = f.n;
            int dv = dist[v];
            forall_inout_edges(e, v) { 
                if ( (f.follow_free && is_matched(e)) || 
                        (!f.follow_free && !is_matched(e)) )
                    continue;

                w = G.opposite( e, v );

                if( reached_HK[w] != phase_number_HK ) {
                    dist[w] = dv + 1; reached_HK[w] = phase_number_HK;
                    if ( free[w] ) augmenting_path_found = true;
                    if ( !augmenting_path_found ) { 
                        bfs_info nf; nf.n = w; nf.follow_free = ! f.follow_free;
                        Q.append(nf);
                    }
                }
                if ( dist[w] == dv + 1 ) useful[e] = phase_number_HK;
            }
        }

        return augmenting_path_found;
    }

    static d_edge find_aug_path_HK( graph& G,
            d_edge f,
            const node_array<bool>& free,
            const node_array<node> & mate,
            node_array<d_edge>& pred,
            const edge_array<int>& useful,
            bool f_matched )
    {
        node w = f.t;
        pred[w] = f;
        if ( free[w] ) return f;
        edge e;
        forall_inout_edges(e,w) {
            if ( (!f_matched && is_matched(e)) || (f_matched && !is_matched(e)) )
                continue;

            d_edge fol; fol.e = e; fol.t = G.opposite( e, w ); fol.s = w;
            if ( pred[fol.t].e != nil || useful[e] != phase_number_HK ) continue;
            d_edge g = find_aug_path_HK( G, fol, free, mate, pred, useful, !f_matched);
            if ( g.e ) return g;
        }
        d_edge fol_nil; fol_nil.e = nil; fol_nil.s=fol_nil.t=nil;
        return fol_nil;
    }

    static bool max_card_bipartite_matching_HK( graph& G,
            const list<node>& A,
            const list<node>& B, 
            node_array<bool>& free,
            node_array<node>& mate )
    {
        node v;
        edge e;
        node_list free_in_A;
        forall(v, A) if ( free[v] ) free_in_A.append(v);

        edge_array<int> useful(G,0);
        node_array<int> dist(G);
        node_array<int> reached_HK(G,0);
        phase_number_HK = 1;

        while( bfs_HK(G, free_in_A, free, mate, useful, 
                    dist, reached_HK ) ) { 

            node_array<d_edge> pred(G);
            forall_nodes( v, G ) { pred[v].e = nil; pred[v].s = nil; pred[v].t = nil; }
            list<d_edge> EL;

            forall(v,free_in_A) { 
                forall_inout_edges(e,v) { 
                    if ( pred[ G.opposite(e,v) ].e == nil && useful[e] == phase_number_HK) { 
                        d_edge fol; fol.e=e; fol.s=v; fol.t=G.opposite(v,e);
                        d_edge f = find_aug_path_HK( G, fol, free, mate, 
                                pred, useful, true);
                        if ( f.e ) { 
                            EL.append(f); 
                            break; 
                        }
                    }
                }
            }

            // augment on disjoint paths
            bool m;
            while (!EL.empty()) { 
                d_edge e = EL.pop();
                free[ e.t ] = false;
                node z = nil;
                m = true;
                while (e.e) { 
                    if ( m ) { 
                        mate[ e.t ] = e.s; 
                        mate[ e.s ] = e.t;
                    } 
                    m = !m;
                    z = e.s; 
                    e = pred[z];
                }
                free[z] = false;
                free_in_A.del(z);
            }
            phase_number_HK++;
        } 
        return true;
    }

    class PopularMatching
    {   
        public:
            PopularMatching( const graph& G_, 
                             const list<node>& A_,
                             const list<node>& B_,
                             const edge_array<int>& rank_
                            ) 
                : G(G_), isLastResort( G, G.number_of_nodes() + A_.size(), false ) 
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
                G.copy_edge_array( rank_, rank );
            }

            bool run( list<edge>& L, int& phase, int maxphase = 2 ) { 
                list<edge> q;
                if ( maxphase < 2 ) 
                    maxphase = 2;
                bool popular = BI_POPULAR_MATCHING_PRIV( maxphase, q, phase );

                L.clear();
                edge e;
                forall( e, q )
                    L.append( G.map( e ) );
                return popular;
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
                        leda::error_handler(999, "POPULAR: illegal rank (non-positive)");
#endif
                    if ( rank[e] > max_rank ) max_rank = rank[e];
                    if ( rank[e] < min_rank ) min_rank = rank[e];
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

            void AddLastResorts() {
                node v,u;
                edge e;
                forall( v, A ) {
                    u = G.new_node();
                    B.append( u );
                    isLastResort[ u ] = true;
                    e = G.new_edge( v, u );
                    rank[ e ] = max_rank + 1;
                }
            }

            void SortGraphEdgesByRank() { 
                G.sort_edges( rank );
            }


            // TODO: in the next function keep a list of applicants which are not already matched
            //       when checking for applicant-completeness use this list and update it

            // main function to compute a popular matching or a not so unpopular one
            bool BI_POPULAR_MATCHING_PRIV( int maxphase, list<edge>& L, int& phase )
            {
                if ( maxphase < 2 )
                    leda::error_handler(999, "POPULAR: maxphase must be at least 2." );

                FindMinAndMaxRank();
                DirectEdgesFromAtoB();
                AddLastResorts();
                SortGraphEdgesByRank();

                // record all edges in specified order and hide them from the graph
                node v; 
                edge e;
                node_array< list<edge> > edgesA( G );
                forall( v, A ) {
                    edgesA[ v ] = list<edge>();
                    forall_out_edges( e , v )
                        edgesA[v].append( e );
                    forall_out_edges( e, v )
                        G.hide_edge( e );
                }

                // keep current edge pointers for each node
                node_array< list_item > currentA( G );
                forall( v, A )
                    currentA[ v ] = edgesA[v].first();

                // now define some stuff relevant to the matching
                node_array<bool> free(G, true);
                node_array<node> mate(G, nil);
                node_array<bool> reached(G, false);
                node_array<bool> always_even( G, true );
                node_array<bool> odd(G);
                stack<node> reached_nodes;

                // main loop
                phase = 0;
                bool added_edges = false, app_complete = false;
                while( !app_complete && phase <= maxphase ) {
                    //std::cout << "phase = " << phase << std::endl;
                    phase++;
                    added_edges = false;
                    app_complete = false;

                    // initialize and find decomposition
                    while( !reached_nodes.empty() ) { 
                        reached[ reached_nodes.top() ] = false; 
                        reached_nodes.pop(); 
                    }
                    partition_graph_node_set_by_bfs( G, A, B, free, mate, reached, odd, reached_nodes );
                    forall_nodes( v, G )
                        if ( !reached[v] || (reached[v] && odd[v] ) )
                            always_even[v] = false;

                    // prune previous iteration edges which cannot participate in maximum matchings
                    prune_edges( G, A, B, reached, odd ); 

                    // add new first choice edges
                    forall( v, A ) { 
                        // if v is not even do nothing
                        if ( !always_even[v] )
                            continue;

                        //std::cout << "checking for new edges out of node " << G[v] << " which is always even." << std::endl;

                        // find the first even house
                        // note: here is the main difference with rank-maximal matchings since we look for the 
                        //       first edge to an even house but not necessarily of the next rank
                        while( currentA[v] != nil ) { 
                            node u = G.opposite( v, edgesA[v].inf(currentA[v]) );
                            if ( always_even[u] )
                                break;
                            currentA[v] = edgesA[v].succ( currentA[v] );
                        }

                        // did we find an edge?
                        if ( currentA[v] == nil ) continue;

                        //std::cout << "edge to first always even house = " << G[ edgesA[v].inf( currentA[v] ) ] << std::endl;

                        // yes, add all appropriate edges
                        int cur_rank = rank[ edgesA[v].inf( currentA[v] ) ];
                        while( currentA[v] != nil && rank[ edgesA[v].inf( currentA[v] ) ] == cur_rank ) {
                            // check if edge should be added
                            node u = G.opposite( v, edgesA[v].inf( currentA[v] ) );
                            if ( always_even[u] ) {
                                G.restore_edge( edgesA[v].inf( currentA[v] ) );
                                //std::cout << "Added edge = " << G[ edgesA[v].inf( currentA[v] ) ] << std::endl;
                                added_edges = true;
                            }
                            currentA[v] = edgesA[v].succ( currentA[v] );
                        }
                    }

                    // quit if no added phase
                    if ( added_edges == false ) { 
                        app_complete = true;
                        forall( v, A )
                            if ( ! is_node_matched(v) ) {
                                app_complete = false;
                                break;
                            }
                        break;
                    }

                    // find maximum matching
                    max_card_bipartite_matching_HK( G, A, B, free, mate );

                    // TODO: should we check that all f-posts are matched?

                    // check if done - is the matching applicant complete?
                    app_complete = true;
                    forall( v, A )
                        if ( ! is_node_matched(v) ) {
                            app_complete = false;
                            break;
                        }
                }

                // debug
                //std::cout << "stopped in phase " << phase << std::endl;
                //std::cout << "matching is applicant complete = " << app_complete << std::endl;
                //std::cout << "matching is popular = " << ( app_complete & (phase==2) ) << std::endl;

                // now return matching
                forall_edges( e, G ) {
                    if ( is_matched(e) ) {
                        if ( !isLastResort[ G.source(e) ] && !isLastResort[ G.target(e) ] )
                            L.append( e );
                    }
                }

                return app_complete && (phase <= 2);
            }




        private:
                
            graph_copy G;
            list<node> A, B;
            edge_map<int> rank;

            int min_rank, max_rank;    
            node_array<bool> isLastResort;
    };


    bool BI_POPULAR_MATCHING( const graph& G, 
            const list<node>& A, const list<node>& B, 
            const edge_array<int>& rank, 
            list<edge>& L )
    {
        PopularMatching pm ( G, A, B, rank );
        int phase;
        return pm.run( L, phase );
    }


    bool BI_APPROX_POPULAR_MATCHING( const graph& G, 
            const list<node>& A, const list<node>& B, 
            const edge_array<int>& rank, 
            int maxphase,
            list<edge>& L,
            int& phase )
    {
        PopularMatching pm ( G, A, B, rank );
        return pm.run( L, phase, maxphase );
    }


    bool BI_APPROX_POPULAR_MATCHING( const graph& G, 
            const list<node>& A, const list<node>& B, 
            const edge_array<int>& rank, 
            list<edge>& L,
            int& phase )
    {
        int maxphase = G.number_of_edges();
        if ( maxphase < 2 ) 
            maxphase = 2;
        PopularMatching pm ( G, A, B, rank );
        return pm.run( L, phase, maxphase );
    }


} // end of namespace

/* ex: set ts=4 sw=4 sts=4 et: */

