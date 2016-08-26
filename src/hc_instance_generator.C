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
#include <LEP/mosp/generator.h>
#include <list>
#include <set>
#include <vector>

#ifdef LEDA_GE_V5
#include <LEDA/graph/graph.h>
#include <LEDA/system/assert.h>
#include <LEDA/numbers/integer.h>
#include <LEDA/core/array.h>
#include <LEDA/core/random_source.h>
#else
#include <LEDA/graph.h>
#include <LEDA/std/assert.h>
#include <LEDA/integer.h>
#include <LEDA/array.h>
#include <LEDA/random_source.h>
#endif // LEDA_GE_V5

namespace mosp
{

#if defined(LEDA_NAMESPACE)
    using leda::graph;
    using leda::node;
    using leda::edge;
    using leda::edge_map;
    using leda::random_source;
    using leda::array;
#endif


    void HighlyCorrelatedInstanceGenerator::GetRandomSubset( int n, int k, leda::array<int>& a )
    {
        if ( k < 1 ) 
            abort();

        a.resize( k );
        leda::array<int> w(n);
        for( int i = 0; i < n; ++i ) 
            w[i] = i;
        for( int i = 0; i < k; ++i ) 
        {
            int j = randomSource(i, n-1);

            int tmp = w[i];
            w[i] = w[j];
            w[j] = tmp;

            a[i] = w[i];
        }
    }

    void HighlyCorrelatedInstanceGenerator::GenerateEdges( leda::graph& G, 
            leda::list<leda::node>& A,
            leda::list<leda::node>& B,
            leda::node_map<int>& capacity,
            leda::edge_map<int>& rank )
    {
        // get random subset of B nodes
        int k = (int) ( EdgeProbability * numPosts );
        if ( k == 0 ) 
            return;


        leda::array<int> ar( k );
        GetRandomSubset( numPosts, k, ar );

        // create edges
        for( int i = 0; i < numApplicants; ++i ) 
            for( int j = 0; j < k; ++j ) 
                G.new_edge( ApplicantToNode[i], PostToNode[ ar[j] ]  );

        // set ranks
        leda::node v; 
        leda::edge e;
        double ch;
        forall( v, A )
        {
            forall_out_edges( e, v ) 
            {
                if ( G.adj_pred( e ) == nil ) 
                    rank[e] = 1;
                else { 
                    int prev_rank = rank[G.adj_pred( e )];
                    randomSource >> ch;
                    rank[e] = (ch <= TieProbability) ? prev_rank : prev_rank + 1;
                }
            }
        }

        // set capacity 1 to everyone
        forall_nodes( v, G ) 
            capacity[v] = Capacity;
    }


} // end of namespace

/* ex: set ts=4 sw=4 sts=4 et: */
