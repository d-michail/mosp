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

/*! \file RANK_MAX_MATCHING.h
 *  \brief Algorithms to compute rank-maximal matchings.
 */

#ifndef RANK_MAX_MATCHING_H
#define RANK_MAX_MATCHING_H

#include <LEP/mosp/config.h>

#ifdef LEDA_GE_V5
#include <LEDA/core/array.h>
#include <LEDA/core/list.h>
#include <LEDA/graph/graph.h>
#include <LEDA/graph/edge_array.h>
#include <LEDA/system/assert.h>
#else
#include <LEDA/array.h>
#include <LEDA/graph.h>
#include <LEDA/list.h>
#include <LEDA/edge_array.h>
#include <LEDA/std/assert.h>
#endif

namespace mosp
{

/*! \brief Compute a rank-maximal matching of a bipartite graph.
 *
 * The function computes a Rank-Maximal matching \f$M\f$ of \f$G\f$, that is a 
 * matching which uses the largest possible number of rank one edges, and 
 * subject to this constraint the largest possible number of rank two edges and so on. 
 * The matching is returned as a list of edges. 
 * The running time is \f$O(r \sqrt{n} m)\f$ where \f$r\f$ is the maximum rank of an edge in the input.
 *  \param G The graph
 *  \param rank A rank function on the edges.
 *  \pre G must be simple, loopfree and bipartite.
 *  \pre rank is a positive integer function on the edges of the graph.
 *  \return A list of edges with the resulting matching.
 *  \ingroup rankmax
 */
leda::list<leda::edge> BI_RANK_MAX_MATCHING( leda::graph& G, 
                          const leda::edge_array<int>& rank );

/*! \brief Compute a rank-maximal matching of a bipartite graph.
 *
 * The function computes a Rank-Maximal matching \f$M\f$ of \f$G\f$, that is a 
 * matching which uses the largest possible number of rank one edges, 
 * and subject to this the largest possible number of rank two edges
 * and so on. 
 * The matching is returned as a list of edges. 
 * This procedure solves the problem by reducing it to the maximum 
 * weight matching problem, and therefore the running time is 
 * \f$O( n^2 ( m + n \log n ))\f$. The second \f$n\f$ comes from the possibly
 * large cost of arithmetic since the reduction involves edge weights
 * which can be as large as \f$n^n\f$. The space requirement is \f$O(mn + n^2)\f$.
 *  \param G The graph
 *  \param rank A rank function on the edges.
 *  \pre G must be simple, loopfree and bipartite.
 *  \pre rank is a positive integer function on the edges of the graph.
 *  \return A list of edges with the resulting matching.
 *  \ingroup rankmax
 */
leda::list<leda::edge> BI_RANK_MAX_MATCHING_MWMR( leda::graph& G, 
                const leda::edge_array<int>& rank );

/*! \brief Compute a rank-maximal matching of a bipartite graph.
 *
 * The function computes a Rank-Maximal matching \f$M\f$ of \f$G\f$, that is a 
 * matching which uses the largest possible number of rank one edges, 
 * and subject to this the largest possible number of rank two edges
 * and so on. 
 * The matching is returned as a list of edges. 
 * This procedure solves the problem by reducing it to the maximum 
 * weight matching problem. The reduction is implicit resulting to a 
 * running time of \f$(r \sqrt{n} m )\f$ where \f$r\f$ is the maximum rank
 * of an edge in the input and linear space.
 *  \param G The graph
 *  \param rank A rank function on the edges.
 *  \pre G must be simple, loopfree and bipartite.
 *  \pre rank is a positive integer function on the edges of the graph.
 *  \return A list of edges with the resulting matching.
 *  \ingroup rankmax
 */
leda::list<leda::edge> DBI_RANK_MAX_MATCHING_MWMR( leda::graph& G, 
                const leda::edge_array<int>& rank );


/*! \brief Compute the profile of a matching.
 * 
 *  The function returns an array \f$ar\f$ of non-negative integers representing the 
 *  number of edges of each rank belonging to the matching. This array is indexed
 *  from \f$1\f$ to \f$n\f$, where \f$n\f$ is the number of vertices of \f$G\f$. 
 *  The running time is \f$O(n)\f$ where \f$n\f$ is the number of nodes of \f$G\f$. 
 *  Note that this procedure does not check whether the list of edges is really
 *  a matching or not.
 *  \param G The input bipartite graph.
 *  \param rank An edge rank function.
 *  \param matching A matching.
 *  \return The profile of the input matching.
 *  \pre rank is a positive integer function on the edges of the graph.
 *  \ingroup rankmax
 */
leda::array<int> BI_RANK_MAX_MATCHING_PROFILE( const leda::graph& G,
                                   const leda::edge_array<int>& rank,
                                   const leda::list<leda::edge>& matching);

// A procedure to check whether a list of edges is 
// a matching, for debugging and testing purposes.
bool DEBUG_is_valid_matching( const leda::graph &G, 
        const leda::list<leda::edge>& matching );

/*! \brief Compute a rank-maximal matching of a bipartite graph with 
 *         capacities on the right side of the bipartite graph.
 *
 * The function computes a Rank-Maximal matching \f$M\f$ of \f$G\f$, that is a 
 * matching which uses the largest possible number of rank one edges, and 
 * subject to this constraint the largest possible number of rank two edges and so on. 
 * Nodes on the right side of the partition may be matched multiple times. This is 
 * why the user must provide a capacity function on the nodes plus a partition of the 
 * nodes such that all edges are from A to B.
 *
 * The matching is returned as a list of edges. The running time is \f$O(n m)\f$.
 * 
 * During the algorithm the input graph is copied and everything happens on the copy.
 *
 *  \param G The graph
 *  \param A Nodes of the left-side partition of the bipartite graph.
 *  \param B Nodes of the right-side partition of the bipartite graph.
 *  \param capacity Node capacities
 *  \param rank A rank function on the edges.
 *  \pre G must be simple, loopfree and bipartite.
 *  \pre rank is a positive integer function on the edges of the graph.
 *  \pre Capacities must be positive.
 *  \pre Capacities of all nodes of the left-side partition must be 1.
 *  \return The list of edges of the computed matching.
 *  \ingroup rankmax
 */
leda::list<leda::edge> BI_RANK_MAX_CAPACITATED_MATCHING( 
            const leda::graph& G, 
            const leda::list<leda::node>& A, 
            const leda::list<leda::node>& B, 
            const leda::node_array<int>& capacity,
            const leda::edge_array<int>& rank 
            );

// A procedure to check whether a list of edges is 
// a matching, for debugging and testing purposes.
bool DEBUG_is_valid_matching( const leda::graph &G, 
            const leda::node_array<int>& capacity,
            const leda::list<leda::edge>& matching );

}  

#endif  // RANK_MAX_MATCHING_H

/* ex: set ts=8 sw=4 sts=4 noet: */


