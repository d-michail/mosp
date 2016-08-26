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

    bool RegionalInstanceGenerator::notIncreaseRank( int j )
    {
        double p = 1 - 1 / exp( Lambda * j );
        double ch;
        randomSource >> ch;
        return ch <= p;
    }

    void RegionalInstanceGenerator::CreateRegion( Region& R, int& remainingPosts, int postsPerRegion, int &firstPostNumber )
    {
        //std::cout << "Creating region" << std::endl;
        R.clear();

        for( int i = 0; i < postsPerRegion && remainingPosts > 0 ; i++ ) 
        {
            if ( i == 0 ) 
            {
                //std::cout << "Creating cluster" << std::endl;
                Cluster tmp; 
                //std::cout << "Adding post number " << firstPostNumber << std::endl;
                tmp.insert( firstPostNumber++ );
                remainingPosts--;
                R.push_back( tmp );
                continue;
            }

            if ( ! notIncreaseRank( i ) ) { 
                //std::cout << "Creating cluster" << std::endl;
                Cluster tmp; 
                //std::cout << "Adding post number " << firstPostNumber << std::endl;
                tmp.insert( firstPostNumber++ );
                R.push_back(tmp);
            }
            else {
                Region::reverse_iterator j = R.rbegin();
                if ( j != R.rend() )
                {
                    //std::cout << "Adding post number " << firstPostNumber << std::endl;
                    (*j).insert( firstPostNumber++ );
                }
            }
            remainingPosts--;
        }
        //std::cout << "end of region" << std::endl;
    }

    void RegionalInstanceGenerator::CreateRegions( std::vector<Region>& R, int regions, int posts  )
    {
        R.clear();
        int firstPostNumber = 0;
        int postsPerRegion = posts/regions;

        while ( postsPerRegion * regions < posts ) { 
            postsPerRegion++;
        }

        //std::cout << "Posts per region = " << postsPerRegion << std::endl;

        for( int i = 0; i < regions; i++ ) 
        {
            Region Q; 
            CreateRegion( Q, posts, postsPerRegion, firstPostNumber );
            R.push_back( Q );
        }
    }

    int RegionalInstanceGenerator::ChooseRegion( std::vector<Region>& R )
    {
        int ran;
        randomSourceI.set_range( 0, R.size() - 1 );
        randomSourceI >> ran;
        //std::cout << "Choosing region " << ran << std::endl;
        return ran;
    }

    void RegionalInstanceGenerator::GenerateEdges( leda::graph& G, 
            leda::list<leda::node>& A,
            leda::list<leda::node>& B,
            leda::node_map<int>& capacity,
            leda::edge_map<int>& rank )
    {
        // create regions
        std::vector<Region> R;
        std::vector<int> PostsLocality;

        CreateRegions( R, Regions, NumPosts() );
        for( int i = 0; i < NumApplicants(); i++ ) 
            PostsLocality.push_back( ChooseRegion( R ) );

        // fix structure for reordering of regions
        std::vector<int> Rordering( Regions, 0 );

        // add edges
        int q; 
        unsigned int j;
        int erank = 1;
        edge e;
        for( int a = 0; a < NumApplicants(); a++ )  // each applicant
        {
            //std::cout << "Constructing edge list for applicant " << a << std::endl;

            erank = 1;

            //std::cout << "Applicant has local region = " << PostsLocality[a] << std::endl;

            // perform a region reordering based on the 
            // post locality
            // (we just set the local region as the second in order)
            for( int i = 0; i < Regions; i++ ) 
                Rordering[i] = i;
            for( int i = PostsLocality[a]; i > 1; i-- ) 
            {
                int tmp = Rordering[i-1];
                Rordering[i-1] = Rordering[i];
                Rordering[i] = tmp;
            }
            //for( int i = 0; i < Regions; i++ ) 
                //std::cout << "Reordered region " << i << " = " << Rordering[i] << std::endl;

            //std::cout << "Constructing edge list" << std::endl;
            for( int i = 0; i < Regions; i++ )  
            {
                q = i;
                j = 0;


                while( q >= 0 ) { 

                    // access array[q][j]
                    //std::cout << "Current q = " << q << std::endl;
                    //std::cout << "Checking size of Region " << Rordering[q] << std::endl;

                    // check that R[q][j] exists
                    if ( R[ Rordering[q] ].size() < j + 1 ) 
                        break;

                    //std::cout << "Region size is at least " << j+1 << " OK!" << std::endl;

                    // add edge from applicant a to q-th Region (R[q]), cluster j

                    //std::cout << "Adding edges to posts in region " << Rordering[q] << " cluster " << j << std::endl;
                    //std::cout << "Cluster has size " << R[ Rordering[q] ][j].size() << std::endl;

                    Cluster::iterator it = R[ Rordering[q] ][j].begin();
                    while( it != R[ Rordering[q] ][j].end() )   
                    {
                        //std::cout << "post is " << (*it) << std::endl;
                        e = G.new_edge( ApplicantToNode[a], PostToNode[ (*it) ] );
                        rank[e] = erank;
                        it++;
                    }

                    j++;
                    q--; 
                    erank++;
                }
            }

        }

        node v; 
        forall_nodes(v, G)
            capacity[v] = Capacity;
    }

} // end of namespace

/* ex: set ts=4 sw=4 sts=4 et: */
