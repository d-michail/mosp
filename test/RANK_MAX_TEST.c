#line 443 "RANK_MAX_MATCHING.lw"
//---------------------------------------------------------------------
// File automatically generated from notangle from RANK_MAX_MATCHING.lw
// 
// mails and bugs: Dimitrios Michail <dimitrios.michail@gmail.com>
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


#line 1296 "RANK_MAX_MATCHING.lw"
#include <iostream>
#include <LEP/rmm/RANK_MAX_MATCHING.h>

int main() {

  int i = 0;

  while (true) {

    std::cout << "\nstarting example " << i++;
    std::cout.flush();
    
    leda::graph G;
    leda::list<leda::node> A,B;
    leda::node v; leda::edge e;

    int n = 1000;  
    int m = n/8 * n/8;  
    float T1,T2;

    // create random graph
    leda::random_bigraph( G, n, n, m, A, B );
    leda::Make_Simple( G );
    leda::Delete_Loops( G );

    // compute random ranks
    leda::edge_array<int> rank(G);
    forall_edges(e,G) rank[e] = leda::rand_int(1,n);

    // now solve
    leda::used_time( T1 );
    leda::list<leda::edge> C = BI_RANK_MAX_MATCHING( G, rank );
    T1 = leda::used_time( T1 );
    leda::array<int> pr = BI_RANK_MAX_MATCHING_PROFILE( G, rank, C );

    // and with reduction to the weight matching
    leda::used_time( T2 );
    leda::list<leda::edge> C1 = BI_RANK_MAX_MATCHING_MWMR( G, rank );
    T2 = leda::used_time( T2 );
    leda::array<int> pr1 = BI_RANK_MAX_MATCHING_PROFILE( G, rank, C1 );

    //std::cout << std::endl << " profile       : " << pr << std::endl;
    //std::cout << " profile (mwmr): " << pr1 << std::endl; 

    // check that two matchings are similar
    assert( RMM_DEBUG_is_valid_matching( G, C ) );
    assert( RMM_DEBUG_is_valid_matching( G, C1 ) );
    assert( pr.size() == pr1.size() );
    for( int j = 1; j <= G.number_of_nodes(); ++j )
    	assert( pr[j] == pr1[j] );
    std::cout << " COMB:" << T1 << " MWMR:" << T2 << " (PASSED)";

  }
  std::cout << std::endl;

  return 0;
}

