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
#include <LEP/mosp/gml.h>
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
    using leda::node_map;
    using leda::random_source;
#endif

    /*! \brief Construct a structured random instance
     *  \param apps The number of applicants
     *  \param posts The number of posts.
     *  \param seed A seed for the random number generator
     */
    StructuredInstanceGenerator::StructuredInstanceGenerator(int apps, int posts, int seed)
        : numApplicants( apps ), numPosts ( posts ), seedUsed( seed )
    {
        randomSource.set_seed( seedUsed );
        randomSourceI.set_seed( seedUsed );
        assert( numApplicants > 0 );
        assert( numPosts > 0 );
    }

    StructuredInstanceGenerator::~StructuredInstanceGenerator()
    {
    }



    /*! \brief Generate a random structured instance in GML format
     *  \param out Output the GML graph in this stream
     */
    void StructuredInstanceGenerator::GenerateGML( std::ostream& o )
    {
        graph G; 
        edge_map<int> rank;
        node_map<int> capacity;
        leda::list< node > A, B;
        GenerateGraph( G, A, B, capacity, rank );
        write_gml( G, A, B, capacity, rank, o  );
    }

    /*! \brief Generate a random structured instance
     *  \param G The graph to return
     *  \param A A list of nodes containing all applicants
     *  \param B A list of nodes containing all posts
     *  \param rank The edge ranks
     */
    void StructuredInstanceGenerator::GenerateGraph( graph& G, 
            leda::list<node>& A, 
            leda::list<node>& B,
            node_map<int>& capacity,
            edge_map<int>& rank )
    {
        InitializeGraph( G, A, B, capacity, rank );
        GenerateEdges( G, A, B, capacity, rank );
    }

    void StructuredInstanceGenerator::InitializeGraph( leda::graph& G,
            leda::list<leda::node>& A, 
            leda::list<leda::node>& B,
            leda::node_map<int>& capacity,
            leda::edge_map<int>& rank )
    { 
        G.clear();
        ApplicantToNode.clear();
        A.clear();
        for( int i = 0 ; i < numApplicants; ++i ) 
        {
            ApplicantToNode[i] = G.new_node();
            A.append( ApplicantToNode[i] );
        }
        PostToNode.clear();
        B.clear();
        for( int i = 0; i < numPosts; ++i )
        {
            PostToNode[i] = G.new_node();
            B.append( PostToNode[i] );
        }
        rank.init( G );
    }

} // end of namespace

/* ex: set ts=4 sw=4 sts=4 et: */
