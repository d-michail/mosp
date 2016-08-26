//---------------------------------------------------------------------
// File automatically generated from notangle from RANK_MAX_MATCHING.lw
// 
// mails and bugs: Dimitrios Michail <dimitris.michail@gmail.com>
//--------------------------------------------------------------------- 
// 
// This program can be freely used in an academic environment
// ONLY for research purposes. Any other use is strictly
// prohibited by the author, without an explicit permission.
//
// Note that this program uses the LEDA library, which is
// NOT free. For more details visit Algorithmic Solutions
// at http://www.algorithmic-solutions.com/
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
// Copyright 2004 - Dimitrios Michail

#include <LEDA/graph.h>
#include <LEDA/array.h>
#include <LEDA/std/assert.h>

#if defined(LEDA_NAMESPACE)
using leda::graph;
using leda::list;
using leda::array;
using leda::node;
using leda::node_list;
using leda::node_array;
using leda::edge;
using leda::edge_array;
#endif

static int total_edge_set_size;

#define is_matched(edge) ( mate[source(edge)] == target(edge) )

typedef struct { bool odd, follow_both, follow_free; node n; } bfs_info;

static void partition_graph_node_set_by_bfs( graph& G,
                                             const list<node>& A,
                                             const list<node>& B,
                                             const node_array<bool>& free,
                                             const node_array<node>& mate,
                                             node_array<bool>& reached,
                                             node_array<bool>& odd,
                                             array<int>& edge_set_size,
                                             const edge_array<int>& rank,
                                             const int phase
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
    
    forall_inout_edges( e , i.n ) { 
      if ( rank [ e ] > phase ) break;


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
                         array<int>& edge_set_size,
                         const edge_array<int>& rank,
                         node_array<bool>& reached,
                         const node_array<bool>& odd,
                         int phase ) 
{
  edge e; node v, w;
  bool v_is_odd, w_is_odd;
  forall( v, A ) 
    {
      forall_inout_edges( e, v ) 
        {
          w = G.opposite( e , v );
          // !! odd(e) is not well defined if reached(e) == false !!
          v_is_odd = reached[ v ] & odd[ v ];
          w_is_odd = reached[ w ] & odd[ w ];

                    if ( rank[e] <= phase-1 ) { 
                      if ( (v_is_odd && ( !reached[ w ] || w_is_odd )) || 
                           (w_is_odd && ( !reached[ v ] || v_is_odd )) ) {
                        G.hide_edge(e);
                        edge_set_size[ rank [ e ] ] --;
                      }
                    }
                    else { // rank[e] > phase-1 
                      if ( !reached[ v ] || !reached[ w ] || v_is_odd ||  w_is_odd  ) {
                        G.hide_edge( e );
                        edge_set_size[ rank[e] ] --;
                        total_edge_set_size --;
                      }
                    }

          
        }
      reached[v] = false;
    }
    forall( v, B ) reached[v] = false;
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
                   node_array<int>& reached_HK,
                   const edge_array<int>& rank, 
                   const int phase )

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
      if ( rank [ e ] > phase ) break;

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
                                const edge_array<int>& rank,
                                const int phase,
                                bool f_matched )
{
  node w = f.t;
  pred[w] = f;
  if ( free[w] ) return f;
  edge e;
  forall_inout_edges(e,w) {
    if ( rank [ e ] > phase ) break;

    if ( (!f_matched && is_matched(e)) || (f_matched && !is_matched(e)) )
      continue;

    d_edge fol; fol.e = e; fol.t = G.opposite( e, w ); fol.s = w;
    if ( pred[fol.t].e != nil || useful[e] != phase_number_HK ) continue;
    d_edge g = find_aug_path_HK( G, fol, free, mate, pred, useful, 
                                 rank, phase, !f_matched);
    if ( g.e ) return g;
  }
  d_edge fol_nil; fol_nil.e = nil; fol_nil.s=fol_nil.t=nil;
  return fol_nil;
}

static bool max_card_bipartite_matching_HK( graph& G,
                                            const list<node>& A,
                                            const list<node>& B, 
                                            node_array<bool>& free,
                                            node_array<node>& mate,
                                            const edge_array<int>& rank,
                                            const int phase )
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
         dist, reached_HK, rank, phase ) ) { 
    
    node_array<d_edge> pred(G);
    forall_nodes( v, G ) { pred[v].e = nil; pred[v].s = nil; pred[v].t = nil; }
    list<d_edge> EL;
    
    forall(v,free_in_A) { 
      forall_inout_edges(e,v) { 
        if ( rank [ e ] > phase ) break;


        if ( pred[ G.opposite(e,v) ].e == nil && useful[e] == phase_number_HK) { 
          d_edge fol; fol.e=e; fol.s=v; fol.t=G.opposite(v,e);
          d_edge f = find_aug_path_HK( G, fol, free, mate, 
                                       pred, useful, rank, phase, true);
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
      node z;
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

static void greedy_heuristic( graph& G,
                              const list<node> A,
                              const list<node> B,
                              const edge_array<int>& rank,
                              const int phase,
                              node_array<bool>& free, 
                              node_array<node>& mate )
{
  node v, w; edge e;
  forall( v , A ) {
    if ( ! free[v] ) continue;
    forall_inout_edges( e , v ) { 
      if ( rank [ e ] > phase ) break;

      w = G.opposite(e , v );
      if ( free[w] ) { 
        free[v] = false; free[w] = false;
        mate[v] = w; mate[w] = v;

        break;
      }
    }
  }
}

list<edge> BI_RANK_MAX_MATCHING( graph& G, const edge_array<int>& rank )
{
    list<node> A, B; 
    node v; edge e;
    node_array<bool> free(G, true);
    node_array<node> mate(G, nil);
    int min_rank = MAXINT, max_rank = 1;
    int phase;

    node_array<bool> reached(G, false);
    node_array<bool> odd(G);

  #if ! defined(LEDA_CHECKING_OFF)
    assert( Is_Bipartite( G, A, B ) && Is_Simple( G ) && Is_Loopfree( G ) );
  #endif

    // direct edges from A to B
    forall( v, A ) 
          forall_in_edges( e, v ) 
                  G.rev_edge( e );

    // find minimum rank, and start from that phase
    // also find maximum rank, in order to quit
    forall_edges( e, G ) {
  #if ! defined(LEDA_CHECKING_OFF)
      if( rank[e] <= 0 ) 
          leda::error_handler(999, "RANK_MAX: illegal rank (non-positive)");
  #endif   
      min_rank = (rank[e] < min_rank)?rank[e]:min_rank;
      max_rank = (rank[e] > max_rank)?rank[e]:max_rank;
    }
    phase = min_rank;

    // create array for keeping the sizes of the edge sets
    // note that each such number is an int.
    // and also initialize it
    // TODO: make this a template ( integer )
    array<int> edge_set_size(max_rank + 1);
    edge_set_size.init( 0 );
    total_edge_set_size = 0;
    forall_edges( e , G ) { 
      edge_set_size[ rank[e] ] ++;
      if( rank[e] > min_rank )  // count from min_rank+1 up to max_rank
        total_edge_set_size ++;
    }

    // sort the edges of the graph, based on the rank
    G.sort_edges( rank );


    // first apply the greedy heuristic
    greedy_heuristic( G, A, B, rank, phase , free, mate );

    // now find a maximal matching in induced subgraph by HK
    max_card_bipartite_matching_HK( G, A, B, free, mate, rank, phase );


  phase++;
  while( phase <= max_rank ) { 
    
            if ( total_edge_set_size == 0 ) break;
            if ( edge_set_size[ phase ] == 0 ) {  
              phase++;
              continue;
            }


        // partition the graph nodes ( Odd, Even, Unreached )
        partition_graph_node_set_by_bfs( G, A, B, free, mate, reached, odd, 
                                         edge_set_size, rank, phase - 1 );

        // prune edge set, and append new edges on induced subgraph
        prune_edges( G, A, B, edge_set_size, rank, reached, odd, phase );
        
        // find maximum cardinality matching in induced graph
        max_card_bipartite_matching_HK( G, A, B, free, mate, rank, phase );

        phase++;
        total_edge_set_size -= edge_set_size[ phase - 1 ];

  }

    list<edge> matched;
    forall_edges( e , G ) 
      if ( is_matched(e) ) 
        matched.append(e);
    
    G.restore_all_edges();

    return matched;

}



array<int> BI_RANK_MAX_MATCHING_PROFILE( const graph& G, 
        const edge_array<int>& rank,
        const list<edge>& matching )
{
        array<int> ar( 1, G.number_of_nodes() ); // use 1..n  
        ar.init( 0 );

        edge e;
        int r;
        forall( e, matching ) {
                r = rank[e];
#if ! defined(LEDA_CHECKING_OFF)
                assert( r >=1 && r <= G.number_of_nodes() );
#endif          
                ar[ r ]++;
        }

        return ar;
}


#include <LEDA/integer.h>
#include <LEDA/templates/mwb_matching.t>

#if defined(LEDA_NAMESPACE)
using leda::integer;
#endif

// compute a rank-maximal matching by a straightforward
// maximum weight matching reduction
list<edge> BI_RANK_MAX_MATCHING_MWMR( graph& G, 
                const edge_array<int>& rank ) 
{

        // check preconditions
        int n = G.number_of_nodes();
        list<node> A,B;
#if ! defined(LEDA_CHECKING_OFF)
        assert( leda::Is_Bipartite( G, A, B ) && 
                leda::Is_Simple( G ) && 
                leda::Is_Loopfree( G ) );
#endif          

        // set edges from A -> B
        edge e;
        node v;
        forall( v, B ) { 
                forall_out_edges( e, v ) {
                        G.rev_edge(e);
                }
        }


        // partition edges by rank
        list<edge>* lrank = new list<edge>[ n + 1 ];
        forall_edges(e, G) { 
#if ! defined(LEDA_CHECKING_OFF)
                assert( rank[e] > 0 && rank[e] <= n );
#endif          
                lrank[ rank[e] ].append( e );
        }


        bool can_fit_in_long = true;

        // build weight function
        edge_array<integer> w( G );
        integer t = 1;
        for( int i = n; i >=1; i-- ) {
                // check if 3 times the current weight fits 
                // in long datatype
                // the LEDA algorithm uses at most 3C numbers
                // where C is the maximum integer weight of an edge
                if ( ( ( 3 * t ).is_long() ) == false )
                        can_fit_in_long = false;
        
                // don't increase weight if no edge
                if ( lrank[i].empty() ) continue;

                forall( e, lrank[i] )
                        w[e] = t;
                t *= n;
        }

        // now delete lrank
        delete [] lrank;


        // now solve appropriately, either with integers or with long
        if ( can_fit_in_long ) {  // solve with long
                edge_array<long> wl( G );
                forall_edges( e, G )
                        wl[e] = w[e].to_long();
                
                return leda::MAX_WEIGHT_BIPARTITE_MATCHING_T<long>( G, A, B, wl );
        }
        else // solve MWM
                return leda::MAX_WEIGHT_BIPARTITE_MATCHING_T<integer>( G, A, B, w );
}


#include <LEDA/edge_set.h>
#include <LEDA/node_set.h>
#include <LEDA/mcb_matching.h>

#if defined(LEDA_NAMESPACE)
using leda::edge_set;
using leda::node_set;
#endif


// return a maximum weight matching from sets vzero and etight
list<edge> 
MWM_from_zero_tight( const graph& G, 
        const node_set& vzero, 
        const edge_set& etight ) 
{ 
        // init and create new graph with two copies
        node v; edge e;
        graph G2;
        node_array< node > av( G ), bv( G );
        edge_array< edge > ae( G );

        // create nodes
        forall_nodes( v, G ) { 
                av[v] = G2.new_node();
                bv[v] = G2.new_node();
        }

        // create edges
        forall_edges( e , G ) { 
                ae[ e ] = G2.new_edge( av[ G.source(e) ] , av[ G.target(e) ] );
                G2.new_edge( bv[ G.source(e) ] , bv[ G.target(e) ] );
        }

        // add extra edges between zero potential nodes
        forall( v, vzero )
                G2.new_edge( av[v], bv[v] );

        // create a reverse index for edges in A
        edge_array< edge > aer( G2, nil ); 
        forall_edges( e, G )
                aer[ ae[ e ] ] = e;
        
        // find max-cardinality by LEDA's function
        list<edge> ret = leda::MAX_CARD_BIPARTITE_MATCHING( G2 );
        
        // return only the edges in A part
        list<edge> rankmax;
        forall( e, ret )
                if ( aer[ e ] != nil )
                        rankmax.push( aer[e] );

        return rankmax;
}

// debugging
bool RMM_DEBUG_is_valid_matching( graph &G, 
        const list<edge>& matching ) { 
        node_array<int> v(G,0);
        edge e;
        forall (e, matching) { 
                v[ G.source(e) ]++;
                v[ G.target(e) ]++;
                if ( v[ G.target(e) ] > 1 ) return false;
        }
        return true;
}




