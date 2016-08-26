//---------------------------------------------------------------------
// File automatically generated using notangle from RANK_MAX_MATCHING.lw
//
// mails and bugs: Dimitrios Michail <dimitrios.michail@gmail.com>
//---------------------------------------------------------------------
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
// Copyright (C) 2004-2005 Dimitrios Michail


#include <iostream>
#include <LEP/rmm/RANK_MAX_MATCHING.h>

int main(  )
{

    int i = 0;

    while ( true ) {

	std::cout << "starting example " << i++ << std::endl;

	leda::graph G;
	leda::list < leda::node > A, B;
	leda::node v;
	leda::edge e;

	int n = 1000;
	int m = n / 8 * n / 8;
	float T1, T2, T3;

	// create random graph
	leda::random_bigraph( G, n, n, m, A, B );
	leda::Make_Simple( G );
	leda::Delete_Loops( G );

	// compute random ranks
	leda::edge_array < int >rank( G );

	forall_edges( e, G ) rank[e] = leda::rand_int( 1, n );

	// now solve
	leda::used_time( T1 );
	leda::list < leda::edge > C = BI_RANK_MAX_MATCHING( G, rank );
	T1 = leda::used_time( T1 );
	leda::array < int >pr = BI_RANK_MAX_MATCHING_PROFILE( G, rank, C );

	// and with reduction to the weight matching
	leda::used_time( T2 );
	leda::list < leda::edge > C1 = BI_RANK_MAX_MATCHING_MWMR( G, rank );
	T2 = leda::used_time( T2 );
	leda::array < int >pr1 = BI_RANK_MAX_MATCHING_PROFILE( G, rank, C1 );

	std::cout << std::endl << " profile       : " << pr << std::endl;
	std::cout << " profile (mwmr): " << pr1 << std::endl; 

	// check that two matchings are similar
	assert( RMM_DEBUG_is_valid_matching( G, C ) );
	assert( RMM_DEBUG_is_valid_matching( G, C1 ) );
	assert( pr.size(  ) == pr1.size(  ) );
	for ( int j = 1; j <= G.number_of_nodes(  ); ++j )
	    assert( pr[j] == pr1[j] );
	std::cout << " COMB:" << T1 << " MWMR:" << T2 << " (PASSED)";
	std::cout << std::endl << std::endl;

    }

    return 0;
}

/* ex: set ts=8 sw=4 sts=4 noet: */
