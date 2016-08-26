#line 415 "RANK_MAX_MATCHING.lw"
//---------------------------------------------------------------------
// File automatically generated from notangle from RANK_MAX_MATCHING.lw
// 
// mails and bugs: Dimitrios Michail <dimitrios.michail@gmail.com>
//--------------------------------------------------------------------- 

#line 450 "RANK_MAX_MATCHING.lw"
#include <LEDA/graph.h>
#include <LEDA/array.h>
#include <LEDA/std/assert.h>

#line 455 "RANK_MAX_MATCHING.lw"
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

#line 473 "RANK_MAX_MATCHING.lw"
static int total_edge_set_size;

#line 490 "RANK_MAX_MATCHING.lw"
#define is_matched(edge) ( mate[source(edge)] == target(edge) )

#line 678 "RANK_MAX_MATCHING.lw"
typedef struct { bool odd, follow_both, follow_free; node n; } bfs_info;

#line 681 "RANK_MAX_MATCHING.lw"
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
#line 703 "RANK_MAX_MATCHING.lw"
list<bfs_info> Q;
node v; edge e;

forall_nodes(v, G ) { // add initial nodes
	if ( ! free[v] ) continue;
	bfs_info i; i.n = v; i.follow_both = true; 
	i.odd = false; i.follow_free = true;
	Q.append( i );
}

#line 721 "RANK_MAX_MATCHING.lw"
  while( ! Q.empty() ) { // now traverse in bfs style
    bfs_info i = Q.pop();
    reached[ i.n ] = true; odd[ i.n ] = i.odd;
    
    forall_inout_edges( e , i.n ) { 
      
#line 802 "RANK_MAX_MATCHING.lw"
if ( rank [ e ] > phase ) break;


#line 728 "RANK_MAX_MATCHING.lw"
      if ( ( i.follow_both || i.follow_free ) && 
      	   !reached[ G.opposite(e,i.n) ] && !is_matched(e) ) { 
	     
#line 742 "RANK_MAX_MATCHING.lw"
	bfs_info nexti; nexti.odd = ! i.odd; nexti.n = G.opposite(e, i.n);
	nexti.follow_both = false; nexti.follow_free = false;
	Q.append( nexti );

#line 731 "RANK_MAX_MATCHING.lw"
      }
      if( ( i.follow_both || ! i.follow_free ) && 
          !reached[ G.opposite(e,i.n) ] && is_matched(e) ) {
	     
#line 749 "RANK_MAX_MATCHING.lw"
	bfs_info nexti; nexti.odd = ! i.odd; nexti.n = G.opposite( e, i.n );
	nexti.follow_both = false; nexti.follow_free = true;
	Q.append( nexti );

#line 735 "RANK_MAX_MATCHING.lw"
      }
    }
  }

#line 695 "RANK_MAX_MATCHING.lw"
}

#line 813 "RANK_MAX_MATCHING.lw"
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

	  
#line 866 "RANK_MAX_MATCHING.lw"
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

#line 834 "RANK_MAX_MATCHING.lw"
	  
	}
      reached[v] = false;
    }
    forall( v, B ) reached[v] = false;
}

#line 896 "RANK_MAX_MATCHING.lw"
static int phase_number_HK;

// an directed edge on an undirected graph
typedef struct { edge e; node s,t; } d_edge;

#line 902 "RANK_MAX_MATCHING.lw"
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
      
#line 802 "RANK_MAX_MATCHING.lw"
if ( rank [ e ] > phase ) break;

#line 931 "RANK_MAX_MATCHING.lw"
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

#line 953 "RANK_MAX_MATCHING.lw"
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
    
#line 802 "RANK_MAX_MATCHING.lw"
if ( rank [ e ] > phase ) break;

#line 969 "RANK_MAX_MATCHING.lw"
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

#line 989 "RANK_MAX_MATCHING.lw"
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
        
#line 802 "RANK_MAX_MATCHING.lw"
if ( rank [ e ] > phase ) break;


#line 1018 "RANK_MAX_MATCHING.lw"
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

#line 767 "RANK_MAX_MATCHING.lw"
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
      
#line 802 "RANK_MAX_MATCHING.lw"
if ( rank [ e ] > phase ) break;

#line 780 "RANK_MAX_MATCHING.lw"
      w = G.opposite(e , v );
      if ( free[w] ) { 
      	
#line 790 "RANK_MAX_MATCHING.lw"
free[v] = false; free[w] = false;
mate[v] = w; mate[w] = v;

#line 783 "RANK_MAX_MATCHING.lw"
	break;
      }
    }
  }
}

#line 498 "RANK_MAX_MATCHING.lw"
list<edge> BI_RANK_MAX_MATCHING( graph& G, const edge_array<int>& rank )
{
  
#line 532 "RANK_MAX_MATCHING.lw"
  list<node> A, B; 
  node v; edge e;
  node_array<bool> free(G, true);
  node_array<node> mate(G, nil);
  int min_rank = MAXINT, max_rank = 1;
  int phase;

  node_array<bool> reached(G, false);
  node_array<bool> odd(G);

#line 502 "RANK_MAX_MATCHING.lw"
  
#line 556 "RANK_MAX_MATCHING.lw"
  assert( Is_Bipartite( G, A, B ) && Is_Simple( G ) && Is_Loopfree( G ) );

  // direct edges from A to B
  forall( v, A ) 
  	forall_in_edges( e, v ) 
		G.rev_edge( e );

  // find minimum rank, and start from that phase
  // also find maximum rank, in order to quit
  forall_edges( e, G ) {
    assert( rank[e] > 0 );
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


#line 504 "RANK_MAX_MATCHING.lw"
  
#line 596 "RANK_MAX_MATCHING.lw"
  // first apply the greedy heuristic
  greedy_heuristic( G, A, B, rank, phase , free, mate );

  // now find a maximal matching in induced subgraph by HK
  max_card_bipartite_matching_HK( G, A, B, free, mate, rank, phase );


#line 506 "RANK_MAX_MATCHING.lw"
  phase++;
  while( phase <= max_rank ) { 
    
#line 615 "RANK_MAX_MATCHING.lw"
    
#line 640 "RANK_MAX_MATCHING.lw"
    if ( total_edge_set_size == 0 ) break;
    if ( edge_set_size[ phase ] == 0 ) {  
      phase++;
      continue;
    }


#line 617 "RANK_MAX_MATCHING.lw"
    // partition the graph nodes ( Odd, Even, Unreached )
    partition_graph_node_set_by_bfs( G, A, B, free, mate, reached, odd, 
    				     edge_set_size, rank, phase - 1 );

    // prune edge set, and append new edges on induced subgraph
    prune_edges( G, A, B, edge_set_size, rank, reached, odd, phase );
    
    // find maximum cardinality matching in induced graph
    max_card_bipartite_matching_HK( G, A, B, free, mate, rank, phase );

    phase++;
    total_edge_set_size -= edge_set_size[ phase - 1 ];

#line 509 "RANK_MAX_MATCHING.lw"
  }

  
#line 654 "RANK_MAX_MATCHING.lw"
  list<edge> matched;
  forall_edges( e , G ) 
    if ( is_matched(e) ) 
      matched.append(e);
  
  G.restore_all_edges();

  return matched;

#line 512 "RANK_MAX_MATCHING.lw"
}



#line 1063 "RANK_MAX_MATCHING.lw"
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
		assert( r >=1 && r <= G.number_of_nodes() );
		ar[ r ]++;
	}

	return ar;
}


#line 1090 "RANK_MAX_MATCHING.lw"
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

#line 1108 "RANK_MAX_MATCHING.lw"
	// check preconditions
	int n = G.number_of_nodes();
	list<node> A,B;
  	assert( leda::Is_Bipartite( G, A, B ) && 
		leda::Is_Simple( G ) && 
		leda::Is_Loopfree( G ) );

#line 1120 "RANK_MAX_MATCHING.lw"
	// set edges from A -> B
	edge e;
	node v;
	forall( v, B ) { 
		forall_out_edges( e, v ) {
			G.rev_edge(e);
		}
	}


#line 1135 "RANK_MAX_MATCHING.lw"
	// partition edges by rank
	list<edge> lrank[ n + 1 ];
	forall_edges(e, G) { 
		assert( rank[e] > 0 && rank[e] <= n );
		lrank[ rank[e] ].append( e );
	}

#line 1149 "RANK_MAX_MATCHING.lw"
	// build weight function
	edge_array<integer> w( G );
	integer t = 1;
	for( int i = n; i >=1; i-- ) {
		// don't increase weight if not edge
		if ( lrank[i].empty() ) continue;

		forall( e, lrank[i] )
			w[e] = t;
		t *= n;
	}

#line 1166 "RANK_MAX_MATCHING.lw"
	// solve MWM
	return leda::MAX_WEIGHT_BIPARTITE_MATCHING_T<integer>( G, A, B, w );
}


#line 1179 "RANK_MAX_MATCHING.lw"
// debugging
bool RMM_DEBUG_is_valid_matching( graph &G, const list<edge>& matching ) { 
	node_array<int> v(G,0);
	edge e;
	forall (e, matching) { 
		v[ G.source(e) ]++;
		v[ G.target(e) ]++;
		if ( v[ G.target(e) ] > 1 ) return false;
	}
	return true;
}




