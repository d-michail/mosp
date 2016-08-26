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
#endif

    bool FSExponentialInstanceGenerator::notIncreaseRank( int j ) 
    {
        double p = 1 - 1 / exp( Lambda * j );
        double ch;
        randomSource >> ch;
        return ch <= p;
    }

    FSExponentialInstanceGenerator::Clustering FSExponentialInstanceGenerator::PartitionPosts( int n )
    {
        Clustering clusters;

        for( int i = 0; i < n; ++i )
        {
            if ( i == 0 ) 
            {
                //std::cout << "Creating new cluster with "<< i << std::endl;
                Cluster tmp; 
                tmp.insert( i );
                clusters.push_back( tmp );
                continue;
            }
    
            if ( ! notIncreaseRank(i) ) { 
                //std::cout << "Creating new cluster" << std::endl;
                //std::cout << "Adding " << i << " to old cluster" << std::endl;
                Cluster tmp; 
                tmp.insert( i );
                clusters.push_back( tmp );
            }
            else { 
                //std::cout << "Adding " << i << " to old cluster" << std::endl;
                Clustering::reverse_iterator j = clusters.rbegin();
                if ( j != clusters.rend() )
                {
                    (*j).insert( i );
                }
            }
        }
        return clusters;
    }

    FSExponentialInstanceGenerator::Clustering::const_iterator FSExponentialInstanceGenerator::ChooseCluster( const Clustering& c )
    {
        Cluster ret;
        int cs = c.size();
        int ran;
        randomSourceI.set_range( 0, cs - 1 );
        randomSourceI >> ran;

        int j = 0;
        Clustering::const_iterator i;
        for(  i = c.begin(); j < ran && i != c.end(); i++, j++ ) {}
        return i;
    }

    void FSExponentialInstanceGenerator::GenerateEdges( leda::graph& G, 
            leda::list<leda::node>& A,
            leda::list<leda::node>& B,
            leda::node_map<int>& capacity,
            leda::edge_map<int>& rank )
    {
        Clustering c = PartitionPosts( numPosts );
        if ( c.size() > 0 )  
        {
            for( int i = 0, erank; i < numApplicants; ++i ) 
            {
                erank = 1;
                for( Clustering::const_iterator q = ChooseCluster(c); q != c.end(); ++q )
                {
                    for( Cluster::iterator j = (*q).begin(); j != (*q).end(); ++j )
                    {
                        rank[ G.new_edge( ApplicantToNode[i], PostToNode[*j] ) ] = erank;
                    }
                    if ((*q).size()>0)
                        ++erank;
                }
            }
        }
        node v; 
        forall_nodes(v, G)
            capacity[v] = 1;
    }
} // end of namespace

/* ex: set ts=4 sw=4 sts=4 et: */
