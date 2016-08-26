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

/*! \file generator.h
 *  \brief Random Structured Instances generator
 *
 *  \ingroup generator
 */

/*! \addtogroup generator
 * 
 *   The library contains four random but structured instance generators. The generators either 
 *   produce a graph in memory or can also output the graph in GML format. Each generator has a
 *   number of parameters (see each class for more details).
 *
 *  \section ex_sec Code Example
 *  \subsection ex_regional Regional Generator
 *  \code
 *
#include <iostream>

#include <LEP/mosp/gml.h>
#include <LEP/mosp/generator.h>


int main( int argc, char* argv[] ) {

    mosp::StructuredInstanceGenerator *gen = 0; 
    gen = new mosp::RegionalInstanceGenerator( 100, 100, 1, 10 , 0.05 );

    leda::graph G;
    leda::list<leda::node> A,B;
    leda::edge_map<int> rank(G);
    leda::node_map<int> capacity(G);

    gen->GenerateGraph( G, A, B, capacity, rank );

    mosp::write_gml( G, A, B, capacity, rank, std::cout );

    delete gen;

    return 0;
}
 *  \endcode
 *
 *  
 *  \{
 */
/** \} */ // end of group generator

#ifndef LEP_GENERATOR_H
#define LEP_GENERATOR_H

#include <LEP/mosp/config.h>
#include <set>
#include <list>
#include <vector>
#include <map>

#ifdef LEDA_GE_V5
#include <LEDA/graph/graph.h>
#include <LEDA/core/random_source.h>
#include <LEDA/system/assert.h>
#include <LEDA/graph/node_map.h>
#include <LEDA/graph/edge_map.h>
#include <LEDA/core/list.h>
#include <LEDA/core/array.h>
#else
#include <LEDA/graph.h>
#include <LEDA/random_source.h>
#include <LEDA/std/assert.h>
#include <LEDA/node_map.h>
#include <LEDA/edge_map.h>
#include <LEDA/list.h>
#include <LEDA/array.h>
#endif

namespace mosp
{


    /*! \brief An instance generator class 
     *
     *  \ingroup generator
     */
    class StructuredInstanceGenerator
    {
        public:

            /*! \brief Construct a structured random instance
             *  \param apps The number of applicants \f$n\f$
             *  \param posts The number of posts \f$m\f$
             *  \param seed A seed for the random number generator
             */
            StructuredInstanceGenerator(int apps, int posts, int seed = default_seed);

            /*! \brief Destructor */
            virtual ~StructuredInstanceGenerator(); 

            /*! \brief Generate a random structured instance in GML format
             *  \param o Output the GML graph in this stream
             */
            void GenerateGML( std::ostream& o );

            /*! \brief Generate a random structured instance
             *  \param G The graph to return
             *  \param A The list of applicants
             *  \param B The list of posts
             *  \param capacity The node capacities
             *  \param rank The edge ranks
             */
            void GenerateGraph( leda::graph& G, 
                    leda::list<leda::node>& A, 
                    leda::list<leda::node>& B,
                    leda::node_map<int>& capacity,
                    leda::edge_map<int>& rank
                    );

            /*! Number of posts that the resulting instance will have. */
            int NumPosts() const { return numPosts; }

            /*! Number of applicants that the resulting instance will have. */
            int NumApplicants() const { return numApplicants; }

        protected:
            static const int default_seed = 13;

            int numApplicants;
            int numPosts;

            int seedUsed;
            leda::random_source randomSource;  // random source
            leda::random_source randomSourceI; // random source in integer mode

            std::map<int, leda::node> ApplicantToNode;
            std::map<int, leda::node> PostToNode;

            void InitializeGraph( leda::graph& G,
                    leda::list<leda::node>& A, 
                    leda::list<leda::node>& B,
                    leda::node_map<int>& capacity,
                    leda::edge_map<int>& rank );

        private:
            virtual void GenerateEdges( leda::graph& G, 
                    leda::list<leda::node>& A,
                    leda::list<leda::node>& B,
                    leda::node_map<int>& capacity,
                    leda::edge_map<int>& rank ) = 0;

    };

    /*! \brief An variable size "exponential" instance generator. 
     *
     *
     *  The variable size random instance generator, while very similar to the 
     *  \link FSExponentialInstanceGenerator fixed size generator \endlink, tries
     *  to capture a slightly different situation. Let \f$p_1, p_2, \ldots, p_m\f$ be a
     *  strict total order, as in the previous generator. We again partition the posts into
     *  clusters of exponentially increasing size. Now, however, clusters are similar but different
     *  for each applicant.
     *
     *  The generator has the following parameters: (a) number of applicants \f$n\f$ and
     *  posts \f$m\f$, (b) instance density parameter \f$d\f$, and (c) parameter \f$\lambda\f$ controlling
     *  the distribution of posts into clusters.
     *  We construct the preference list of an applicant \f$a \in A\f$ as follows. Our goal is to add
     *  an edge from \f$a\f$ to the first \f$m \cdot d\f$ posts. We initially add edge \f$(a, p_1)\f$ with
     *  rank 1. When adding edge \f$(a, p_i)\f$ we decide the rank based on the parameter \f$\lambda\f$.
     *  We choose with probability \f$\frac{1}{e^{\lambda i}}\f$ whether to increase the rank by
     *  one or use the same rank as edge \f$(a, p_{i-1})\f$.
     *
     *  \ingroup generator
     */
    class VSExponentialInstanceGenerator : public StructuredInstanceGenerator
    {
        public:
            /*! \brief Create a new generator
             *
             *  \param apps Number of applicants \f$n\f$.
             *  \param posts Number of posts \f$m\f$.
             *  \param Density Density parameter \f$d\f$.
             *  \param lambda Parameter \f$\lambda\f$.
             */
            VSExponentialInstanceGenerator( int apps, int posts, double Density, double lambda ) : 
                StructuredInstanceGenerator( apps, posts ), EdgeProbability( Density ), Lambda( lambda )
            {
                assert( EdgeProbability >= 0.0 && EdgeProbability <= 1.0 );
            }

            /*! \brief Create a new generator
             *
             *  \param apps Number of applicants \f$n\f$.
             *  \param posts Number of posts \f$m\f$.
             *  \param Density Density parameter \f$d\f$.
             *  \param lambda Parameter \f$\lambda\f$
             *  \param seed A seed for the random number generator
             */
            VSExponentialInstanceGenerator( int apps, int posts, double Density, double lambda, int seed ) : 
                StructuredInstanceGenerator( apps, posts, seed ), EdgeProbability( Density ), Lambda( lambda )
            {
                assert( EdgeProbability >= 0.0 && EdgeProbability <= 1.0 );
            }

            ~VSExponentialInstanceGenerator()
            {
            }

        private:

            virtual void GenerateEdges( leda::graph& G, 
                    leda::list<leda::node>& A,
                    leda::list<leda::node>& B,
                    leda::node_map<int>& capacity,
                    leda::edge_map<int>& rank );

            // probability of an edge beeing there
            const double EdgeProbability;
            const double Lambda;

            // private implementation
            bool notIncreaseRank( int j );
    };

    /*! \brief An fixed size "exponential" instance generator. 
     *
     * In several real-world assignment situations, the number of available positions (with capacities) is
     * relatively low compared to the applicants.
     * Moreover, there exists a total ordering \f$p_1, p_2, \ldots, p_m\f$ of the positions which is known
     * in advance to all applicants (perhaps from an organization responsible for ranking the posts).
     * Since people usually have a very strong opinion about their top choices and relatively weaker opinion
     * about their last choices, this total ordering is strict at the beginning
     * but allows equality towards the end.
     * Based on this observation we divide posts into clusters of exponentially increasing size. These
     * clusters are the same for all applicants. We maintain the following invariant. If an applicant
     * assigns rank \f$i\f$ to a post of cluster \f$P_j\f$, then the applicant ranks all posts of the same
     * cluster \f$P_j\f$ as rank \f$i\f$.
     *
     * Not all applicants have the same self-confidence, translating to some
     * applicants choosing to discart high choices completely,  hoping to get
     * an average one. We emulate this by assigning uniformly at random the first
     * cluster that each applicant will rank. From then on the applicant ranks
     * all remaining clusters.
     * 
     * Note that the last property of our generator can be justified by
     * completely different examples as well. Suppose that a store wants to assign
     * products to shelfs and we view the problem from the products viewpoint.
     * All products want to be assigned to the best shelfs of the store. But not
     * all products produce the same profit for the store-owner. Thus, the store
     * owner is highly likely to restrict the accessibility of these selfs to a
     * few products. Remaining products start biding from a lower quality shelf.
     * 
     * We use the following parameters: (a) number of
     * applicants \f$n\f$ and number of posts \f$m\f$, and (b) a parameter
     * \f$\lambda\f$ controlling the distribution of posts into clusters.
     * More precisely, we construct clusters using the following strategy. We assign
     * post \f$p_1\f$ to the first cluster. Then for each post \f$p_i\f$ we choose with probability
     * \f$\frac{1}{e^{ \lambda \cdot i }}\f$ to construct a new cluster. Otherwise, we
     * assign post \f$p_i\f$ to the last constructed cluster.
     * After the cluster construction
     * each applicant chooses uniformly at random the first cluster to rank.
     *
     *  \ingroup generator
     */
    class FSExponentialInstanceGenerator : public StructuredInstanceGenerator
    {
        public:
            /*! \brief Create a new generator
             *
             *  \param apps Number of applicants \f$n\f$.
             *  \param posts Number of posts \f$m\f$.
             *  \param lambda Parameter \f$\lambda\f$
             */
            FSExponentialInstanceGenerator( int apps, int posts, double lambda ) : 
                StructuredInstanceGenerator( apps, posts ), Lambda( lambda )
            {
            }

            /*! \brief Create a new generator
             *
             *  \param apps Number of applicants \f$n\f$.
             *  \param posts Number of posts \f$m\f$.
             *  \param lambda Parameter \f$\lambda\f$
             *  \param seed A seed for the random number generator
             */
            FSExponentialInstanceGenerator( int apps, int posts, double lambda, int seed ) : 
                StructuredInstanceGenerator( apps, posts, seed ), Lambda( lambda )
            {
            }

            ~FSExponentialInstanceGenerator()
            {
            }

        private:

            virtual void GenerateEdges( leda::graph& G, 
                    leda::list<leda::node>& A,
                    leda::list<leda::node>& B,
                    leda::node_map<int>& capacity,
                    leda::edge_map<int>& rank );

            const double Lambda;

            // private implementation
            typedef std::set<int> Cluster;
            typedef std::list< Cluster > Clustering;

            Clustering PartitionPosts( int n );
            Clustering::const_iterator ChooseCluster( const Clustering& c );

            // private implementation
            bool notIncreaseRank( int j );
    };



    /*! \brief A highly correlated instance generator.
     *
     *
     * Highly correlated instances are problem instances where most of the
     * applicants have a global and consistent knowledge of the posts
     * reputation.
     * Posts are already ranked, from an outside source,
     * in some particular order \f$p_1, p_2, \ldots, p_m\f$. This particular ranking is
     * well known to all applicants and is gradly accepted, perhaps with a few minor adjustments
     * per applicant.
     * The goal is to have preferences which are based on the intuition
     * that if two posts \f$p_i\f$ and \f$p_j\f$ are far in
     * the global ranking from each other then it should be highly likely
     * that they are far away in each applicant's ranking as well.
     *
     * We model such instances based on the following parameters:
     *     - number of applicants \f$n\f$,
     *     - number of posts \f$m\f$,
     *     - a probability \f$d\f$ than an edge exists in our instance,
     *     - a probability \f$t\f$ that an edge is tied to its predecessor, and
     *     - capacity \f$c\f$ for all posts.
     *
     * We create the instance in the following way. For each applicant \f$a\f$
     * we randomly sample with probability \f$d\f$ all posts, resulting in
     * a subset \f$\{ p'_1, p'_2, \ldots\}\f$. We sort this subset
     * of posts based on the total order \f$p_1, p_2, \ldots, p_m\f$.
     * For each post \f$p'_i\f$ we create an edge \f$(a, p'_i)\f$. We assign
     * ranks as follows. We begin by giving edge \f$(a,p'_1)\f$ rank 1 and
     * then we choose with probability \f$t\f$ whether to assign edge
     * \f$(a, p'_2)\f$ the same rank as \f$(a, p'1)\f$ or to increase it. We
     * continue this process for all remaining edges.
     *
     * We call them highly correlated since preferences are highly related among applicants.
     * These instances are particularly difficult with respect to popular matchings.
     *
     *  \ingroup generator
     */
    class HighlyCorrelatedInstanceGenerator : public StructuredInstanceGenerator
    {
        public:
            /*! \brief Create a new generator
             *
             *  \param apps Number of applicants \f$n\f$.
             *  \param posts Number of posts \f$m\f$.
             *  \param Density Density parameter \f$0 \le d \le 1\f$
             *  \param TieProp Tie probability \f$0 \le t \le 1\f$
             *  \param capacity Posts capacity \f$c\f$
             */
            HighlyCorrelatedInstanceGenerator( int apps, int posts, double Density, double TieProp, int capacity ) 
                : StructuredInstanceGenerator( apps, posts ), EdgeProbability( Density ), 
                  TieProbability( TieProp ), Capacity(capacity)
            {
                assert( EdgeProbability >= 0.0 && EdgeProbability <= 1.0 );
                assert( TieProbability >= 0.0 && TieProbability <= 1.0 );
                assert( Capacity >= 1 );
            }

            /*! \brief Create a new generator
             *
             *  \param apps Number of applicants \f$n\f$
             *  \param posts Number of posts \f$m\f$
             *  \param Density Density parameter \f$0 \le d \le 1\f$
             *  \param TieProp Tie probability \f$0 \le t \le 1\f$
             *  \param capacity Posts capacity \f$c\f$
             *  \param seed Seed value for the random number generator
             */
            HighlyCorrelatedInstanceGenerator( int apps, int posts, double Density, double TieProp, int capacity, int seed ) 
                : StructuredInstanceGenerator( apps, posts, seed ), EdgeProbability( Density ), 
                  TieProbability( TieProp ), Capacity( capacity )
            {
                assert( EdgeProbability >= 0.0 && EdgeProbability <= 1.0 );
                assert( TieProbability >= 0.0 && TieProbability <= 1.0 );
                assert( Capacity >= 1 );
            }

            ~HighlyCorrelatedInstanceGenerator()
            {
            }

        private:

            virtual void GenerateEdges( leda::graph& G, 
                    leda::list<leda::node>& A,
                    leda::list<leda::node>& B,
                    leda::node_map<int>& capacity,
                    leda::edge_map<int>& rank );


            // probability of an edge beeing there
            const double EdgeProbability;
            // probability of an edge beeing tied to its predecessor
            const double TieProbability;

            // fixed posts capacity
            int Capacity;

            // get a random subset from the set {0,1,...,n-1}
            void GetRandomSubset( int n, int k, leda::array<int>& a );   
    };


    /*! \brief Regional instance generator
     *
     * Regional instances are best described using as example the assignment of
     * students to universities. The generator tries to capture preferences which are a mixture
     * of (a) a global and well accepted ranking of available posts and (b) stricty personal and emotional
     * choices.
     *
     * We assume that our instance has a number of regions \f$r\f$ which is given as a parameter. The
     * \f$m\f$ posts are divided roughly equally into the regions, thus, each region has \f$m/r\f$ posts
     * except possibly the last. Each post has a maximum capacity denoted by \f$c\f$. Let \f$n\f$ denote
     * the number of applicants and let \f$\lambda\f$ be a parameter which will affect the various
     * distributions.
     * 
     * The regions are sorted in order of preference. We assume that
     * this ordering is valid for all applicants (based for example on some global
     * ranking) except for one region for each applicant. The exception is the region
     * that each applicant belongs to. We assign applicants to regions uniformly at
     * random. Let the ordering of the regions be \f$R_1, R_2, \ldots, R_r\f$.
     * Inside each region, posts are partitioned into clusters of exponentially increasing size.
     * For region \f$R_i\f$ we denote as \f$R_i^1\f$ the set of best posts, \f$R_i^2\f$ the second
     * best and so on. This distribution is based on the parameter \f$\lambda\f$, in a similar fashion
     * as in the other \link FSExponentialInstanceGenerator generators \endlink.
     * 
     * Let us now consider an applicant which belongs to region \f$1 \le j \le r\f$. The
     * following matrix shows the rank of edges between the applicant and the set of
     * employers \f$R_k^l\f$.
    \f[
    \begin{array}{c|c|c|c|c|c}
     & R^1 & R^2 & R^3 & R^4 & \ldots\\
      \hline
       R_1 & 1  & 3  & 6  & 10  & \\
        \hline
         R_j & 2  & 5  & 9  &   & \\
          \hline
           R_2 & 4  & 8  &   &   & \\
            \hline
             R_3 & 7  &   &   &   & \\
              \hline
               \vdots& &   &   &   & \ddots \\
               \end{array}
               \f]
     * In other words each applicant tries to balance between the global ranking of the regions, her/his
     * local region and the ranking of the clusters inside each region.
     * 
     *
     *  \ingroup generator
     */
    class RegionalInstanceGenerator : public StructuredInstanceGenerator
    {
        public:
            /*! \brief Create a new generator
             *
             *  \param apps Number of applicants \f$n\f$
             *  \param posts Number of posts \f$m\f$
             *  \param capacity Capacity of each post \f$c\f$
             *  \param regions Number of regions \f$r\f$
             *  \param lambda Parameter for exponential ordering \f$\lambda\f$
             */
            RegionalInstanceGenerator( int apps, int posts, int capacity, int regions, double lambda ) 
                : StructuredInstanceGenerator( apps, posts ), 
                  Capacity(capacity), Regions(regions), Lambda(lambda) 
            {
                assert( Capacity >= 1 );
                assert( Regions >= 1 );
                assert( Lambda > 0.0 );
            }

            /*! \brief Create a new generator
             *
             *  \param apps Number of applicants \f$n\f$
             *  \param posts Number of posts \f$m\f$
             *  \param capacity Capacity of each post \f$c\f$
             *  \param regions Number of regions \f$r\f$
             *  \param lambda Parameter for exponential ordering \f$\lambda\f$
             *  \param seed Seed value for the random number generator
             */
            RegionalInstanceGenerator( int apps, int posts, int capacity, int regions, double lambda, int seed ) 
                : StructuredInstanceGenerator( apps, posts, seed ), 
                  Capacity(capacity), Regions(regions), Lambda(lambda) 
            {
                assert( Capacity >= 1 );
                assert( Regions >= 1 );
                assert( Lambda > 0.0 );
            }

            ~RegionalInstanceGenerator()
            {
            }

        private:

            // private implementation
            typedef std::set<int> Cluster;
            typedef std::vector< Cluster > Region;

            bool notIncreaseRank( int j );
            void CreateRegion( Region& R, int& remainingPosts, int postsPerRegion, int& firstPostNumber );
            void CreateRegions( std::vector<Region>& R, int regions, int posts  );
            int  ChooseRegion( std::vector<Region>& R );


            virtual void GenerateEdges( leda::graph& G, 
                    leda::list<leda::node>& A,
                    leda::list<leda::node>& B,
                    leda::node_map<int>& capacity,
                    leda::edge_map<int>& rank );


            // private
            const int Capacity;
            const int Regions;
            const double Lambda;
    };

}

#endif  // LEP_GENERATOR_H

/* ex: set ts=4 sw=4 sts=4 et: */


