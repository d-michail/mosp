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


/*! \file mosp.h
 *  \brief General LEP interface.
 */

/*! \mainpage Library for Matchings with One Sided Preferences
 *
 *  \section intro_sec Introduction
 * 
 *  <p>
 *  In <b>M</b>atchings with <b>O</b>ne <b>S</b>ided <b>P</b>references 
 *  we are given a bipartite
 *  graph \f$G(V,E)\f$ where \f$V = \mathcal{A} \cup \mathcal{P}\f$. Set \f$\mathcal{A}\f$ denotes 
 *  a set of applicants and set \f$\mathcal{P}\f$ of posts. Each applicant
 *  in \f$\mathcal{A}\f$ submits a preference list (a ranking of a subset
 *  of the posts, possibly including ties). Edges which are choice \f$i\f$
 *  are called rank \f$i\f$ edges.
 *
 *  <p>
 *  Given this setting we may ask for matchings which satisfy different
 *  optimization criteria. This library contains code for the two following
 *  such criteria. 
 * 
 *  \subsection alg_rankmaximal Rank-Maximal Matchings
 *
 *  A rank-maximal matching is a matching which contains the maximum
 *  number of first choice edges. Given that constraint it contains the 
 *  maximum number of second choice edges and so on.
 *  A rank-maximal matching can be computed in polynomial time. <b>libMOSP</b>
 *  contains three implementations which compute such a matching:
 *
 *  - An \f$O( \min(n+r, r \sqrt{n} ) m )\f$ algorithm where \f$r\f$ is the maximum rank
 *    of an edge in the instance. See <a href="http://portal.acm.org/citation.cfm?doid=1198513.1198520">here</a> for a 
 *    description of the algorithm.
 *  - An algorithm with the same running time but based on a different approach
 *    (reducing the problem to the weighted matching problem). 
 *    See <a href="http://dx.doi.org/10.1016/j.tcs.2007.08.004">here</a> for more details.
 *  - Again an algorithm which reduces the problem to the weighted matching
 *    but this time with slower running time \f$O( n^2 ( m + n \log n ) )\f$ and 
 *    more space \f$O(mn + n^2)\f$. The extra \f$n\f$ comes from the possible cost of arithmetic
 *    since the algorithm handles numbers up to \f$O(n^n)\f$.
 *
 *  Except for the above, <b>libMOSP</b> contains an implementation of a rank-maximal matching
 *  algorithm with capacities. In this case the nodes of the right-side partition of the bipartite
 *  graph may have capacities larger that 1, i.e. they may be matched more than once. The library
 *  contains: 
 *
 *  - An \f$O( n m)\f$ algorithm. See <a href="http://portal.acm.org/citation.cfm?doid=1198513.1198520">here</a>
 *    for more details.
 *
 *  \subsection alg_popular Popular Matchings
 *
 *  We say that a matching \f$M'\f$ is more popular than matching \f$M\f$ if 
 *  more applicants prefer \f$M'\f$ than those who prefer \f$M\f$. Applicants
 *  who are indifferent are ignored. A matching \f$M\f$ is popular if there is 
 *  no other matching which is more popular.
 *
 *  Popular matchings can also be computed in polynomial time (see 
 *  <a href="http://portal.acm.org/citation.cfm?id=1070432.1070491">here</a> for more details), 
 *  but unfortunately they do not always exist. In that case we wish to compute 
 *  matchings which approximate the least unpopular matching (see
 *  <a href="http://www.springerlink.com/content/e882513747l80372">here</a> for more details).
 *
 *  libMOSP contains two algorithms.
 *
 *  - An \f$O( \sqrt{n} m )\f$ algorithm from <a href="http://portal.acm.org/citation.cfm?id=1070432.1070491">here</a> which decides if the instance has a popular matching or not (and possibly
 *    returns one).
 *  - An algorithm which is a variation of the previous one and is guarantied to return
 *    a result even if the instance does not admit a popular matching. See <a href="http://dx.doi.org/10.1007/978-3-540-69903-3_13">here</a>
 *    for a more detailed description.
 *
 *  libMOSP also provides routines to compute the so called "unpopularity factor" and the 
 *  "unpopularity margin" of a matching (see McCutchen 2007).
 *
 *  \subsection Instance Generators
 *
 *  The library also contains 4 random structured instance \ref generator "generators".
 *
 *  \section req_sec Requirements
 *  This implementation is written in C++ and uses 
 *  <a href="http://www.algorithmic-solutions.com/enleda.htm">LEDA</a>. 
 *  The structure of the package follows that of a LEDA extension package 
 *  (<a href="http://www.algorithmic-solutions.com/enleps.htm">LEP</a>).
 *
 *  \section platf_sec Supported Platforms
 *  This package has been tested on the following platforms:
 *  -# gcc 3.x, 4.0.x and 4.1.x under Linux 
 *  -# gcc 3.x under SunOS 5.9
 *  -# gcc 3.x under Cygwin
 *
 *  but it may work on others too.
 *
 *  \section lic_sec License
 *
 *  \verbatim
    This program can be freely used in an academic environment
    ONLY for research purposes, subject to the following restrictions:
 
    1. The origin of this software must not be misrepresented; you must not
       claim that you wrote the original software. If you use this software
       an acknowledgment in the product documentation is required.
    2. Altered source versions must be plainly marked as such, and must not be
       misrepresented as being the original software.
    3. This notice may not be removed or altered from any source distribution.
 
    Any other use is strictly prohibited by the author, without an explicit
    permission.
 
    This software is distributed in the hope that it will be useful,
    but WITHOUT ANY WARRANTY; without even the implied warranty of
    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.
    \endverbatim
 *
 *  Note that this package uses LEDA, which is 
 *  <a href="http://www.algorithmic-solutions.com/enledalizenzen.htm">not free</a>.
 *
 *  \section news_sec News
 *
 *  - 01 Feb 2010: v0.7 released
 *      - Added implementation of popular matchings and variations
 *      - Added random structures instance generators
 *      - Support LEDA 6.0 on linux platforms.
 *  - 24 Apr 2005: v0.6 released
 *      - Support LEDA 5.0 on win32 platforms with bcc32
 *  - 23 Feb 2005: v0.5 released
 *      - Support Borland C++ bcc32 (only LEDA 4.5 or earlier)
 *      - Minor fixes
 *  - 11 Feb 2005: v0.3 released
 *      - Support for LEDA-5.0 added, see INSTALL file
 *  - 19 Oct 2004: v0.2 released
 *  - 23 Sep 2004: v0.1 released containing implementation of rank-maximal matchings
 *
 *  \section down_sec Download
 *
 *  - Source package (v0.7). [<a href="subpages/libmosp/libmosp-0.7.tar.gz">tar.gz</a>]
 *  - Source package (v0.6). [<a href="subpages/rmm/rmm-0.6.tar.gz">tar.gz</a>]
 *  - Source package (v0.5). [<a href="subpages/rmm/rmm-0.5.tar.gz">tar.gz</a>]
 *  - Source package (v0.3). [<a href="subpages/rmm/rmm-0.3.tar.gz">tar.gz</a>]
 *  - Source package (v0.2). [<a href="subpages/rmm/rmm-0.2.tar.gz">tar.gz</a>]
 *
 *  \section ex_sec Code Examples
 *  \subsection ex_rankmaximal Rank-Maximal Matchings
 *  \code
 *
 *  #include <LEP/mosp/mosp.h>
 *
 *  int main() {
 *
 *      leda::graph G;
 *
 *      // construct simple, loopfree, bipartite graph G 
 *
 *      leda::edge_array<int> rank(G, 1);
 *
 *      // fill up ranks
 *
 *      leda::list< leda::edge > M = mosp::BI_RANK_MAX_MATCHING( G, rank );
 *
 *	// do something with M
 *
 *      return 0;
 *  } 
 *  \endcode
 *
 * \subsection ex_popular Popular Matchings
 * \code
 *
 * #include <LEP/mosp/mosp.h>
 *
 * int main() {
 *
 *      leda::graph G;
 *
 *      // construct simple, loopfree, bipartite graph G 
 *
 *      leda::list< leda::node > A, B;
 *
 *      // add left partition nodes to A, right partition nodes to B
 *
 *      leda::edge_array<int> rank(G, 1);
 *
 *      // fill up ranks
 *
 *      leda::list< leda::edge > M;
 *      bool is_popular = mosp::BI_POPULAR_MATCHING( G, A, B, rank, M );
 *
 *	    // do something with M
 *
 *      return 0;
 * }
 * \endcode
 *  \subsection ex_notunpopular Popular Approximation
 *  \code
 *
 *  #include <LEP/mosp/mosp.h>
 *
 *  int main() {
 *
 *      leda::graph G;
 *
 *      // construct simple, loopfree, bipartite graph G 
 *
 *      leda::list< leda::node > A, B;
 *
 *      // add left partition nodes to A, right partition nodes to B
 *
 *      leda::edge_array<int> rank(G, 1);
 *
 *      // fill up ranks
 *
 *      int phase;
 *      leda::list< leda::edge > M;
 *      bool is_popular = mosp::BI_APPROX_POPULAR_MATCHING( G, A, B, rank, M, phase );
 *
 *      int unpopularity_factor;
 *      if ( mosp::BI_UNPOPULARITY_FACTOR( G, A, B, rank, M, unpopularity_factor ) )
 *	        std::cout << "unpopularity factor = " << unpopularity_factor << std::endl;
 *
 *	    // do something with M
 *
 *      return 0;
 *  } 
 *  \endcode

 *
 */

/*! \namespace mosp
 *  \brief The main package namespace
 */

/*! \defgroup rankmax Rank-Maximal Matchings
 *  \defgroup popular Popular Matchings
 *  \defgroup generator Random Structured Instance Generators 
 */

#ifndef MOSP_H
#define MOSP_H

#include <LEP/mosp/RANK_MAX_MATCHING.h>
#include <LEP/mosp/POPULAR.h>

#endif  // MOSP_H

/* ex: set ts=4 sw=4 sts=4 et: */
