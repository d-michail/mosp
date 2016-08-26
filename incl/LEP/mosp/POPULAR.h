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

/*! \file POPULAR.h
 *  \brief Algorithms for popular matchings
 */

#ifndef LEP_POPULAR_H
#define LEP_POPULAR_H

#include <LEP/mosp/config.h>

#ifdef LEDA_GE_V5
#include <LEDA/graph/graph.h>
#include <LEDA/core/list.h>
#include <LEDA/graph/edge_array.h>
#include <LEDA/system/assert.h>
#include <LEDA/core/string.h>
#else
#include <LEDA/graph.h>
#include <LEDA/list.h>
#include <LEDA/edge_array.h>
#include <LEDA/std/assert.h>
#include <LEDA/string.h>
#endif

#include <LEP/mosp/graphcopy.h>

namespace mosp
{

    /*! \brief Compute a popular matching. 
     *
     *  During the algorithm the graph is copied and everything happens on the copy. The
     *  algorithm has a running time of \f$O(\sqrt{n}m)\f$.
     *
     *  \param G A bipartite graph.
     *  \param A The list of nodes on the left side of the bipartite graph.
     *  \param B The list of nodes on the right side of the bipartite graph.
     *  \param rank An edge array. Each entry corresponds to the rank of an edge.
     *  \param L A list of edges which after the algorithm will contain the computed matching.
     *  \return True if the matching is popular, false otherwise.
     *  \ingroup popular
     */
    bool BI_POPULAR_MATCHING( const leda::graph& G, 
	    const leda::list<leda::node>& A, 
	    const leda::list<leda::node>& B, 
	    const leda::edge_array<int>& rank, 
	    leda::list<leda::edge>& L );

    /*! \brief Compute an approximate popular matching. 
     *
     *  During the algorithm the graph is copied and everything happens on the copy.
     *
     *  \param G A bipartite graph.
     *  \param A The list of nodes on the left side of the bipartite graph.
     *  \param B The list of nodes on the right side of the bipartite graph.
     *  \param rank An edge array. Each entry corresponds to the rank of an edge.
     *  \param maxphase The maximum phase that the algorithm will try to compute a matching.
     *                  As you increase the phases the matching's popularity will drop but the
     *                  chance of finding a matching that is approximate popular increases.
     *  \param L A list of edges which after the algorithm will contain the computed matching.
     *  \param phase A value which contains the number of phases that the algorithm needed to 
     *               compute the resulting matching. It will be at most the value of the 
     *               parameter maxphase.
     *  \return True if the matching is popular, false otherwise.
     *  \ingroup popular
     */
    bool BI_APPROX_POPULAR_MATCHING( const leda::graph& G, 
	    const leda::list<leda::node>& A, 
	    const leda::list<leda::node>& B, 
	    const leda::edge_array<int>& rank, 
	    int maxphase,
	    leda::list< leda::edge >& L,
	    int& phase );

    /*! \brief Compute an approximate popular matching. 
     *
     *  During the algorithm the graph is copied and everything happens on the copy.
     *
     *  \param G A bipartite graph.
     *  \param A The list of nodes on the left side of the bipartite graph.
     *  \param B The list of nodes on the right side of the bipartite graph.
     *  \param rank An edge array. Each entry corresponds to the rank of an edge.
     *  \param L A list of edges which after the algorithm will contain the computed matching.
     *  \param phase A value which contains the number of phases that the algorithm needed to 
     *               compute the resulting matching. A value of 2 means that the algorithm found
     *               a popular matching.
     *  \return True if the matching is popular, false otherwise.
     *  \ingroup popular
     */
    bool BI_APPROX_POPULAR_MATCHING( const leda::graph& G, 
	    const leda::list<leda::node>& A, 
	    const leda::list<leda::node>& B, 
	    const leda::edge_array<int>& rank, 
	    leda::list< leda::edge >& L,
	    int& phase );


    /*! \brief Compute the unpopularity factor of a matching. 
     *  
     *  The unpopularity factor is 
     *  based on the definition with the multiplicities. See McCutchen 2007.
     *
     *  This function might change the order of the graph edges in the adjacency lists.
     *
     *  \param G A bipartite graph.
     *  \param A The list of nodes on the left side of the bipartite graph.
     *  \param B The list of nodes on the right side of the bipartite graph.
     *  \param rank An edge array. Each entry corresponds to the rank of an edge.
     *  \param M The matching to test in the following form. A list of edges.
     *  \param factor Contains the unpopularity factor if it is finite. Undefined otherwise.
     *  \pre   All edges are directed from A to B.
     *  \pre   The adjacency lists of the graph must be sorted according to the rank parameter. This
     *         means that in LEDA someone has to call G.sort_edges( rank ) before calling this function.
     *  \return True if the matching has a finite unpopularity factor, false if infinite.
     *  \ingroup popular
     */
    bool BI_UNPOPULARITY_FACTOR( graph& G,
	    const list<node>& A,  const list<node>& B,
	    const edge_array<int>& rank,
	    const list<edge>& M,
	    int &factor );

    /*! \brief Compute a popular matching in a capacitated instance. 
     *
     *  During the algorithm the graph is copied and everything happens on the copy. 
     *
     *  \param G A bipartite graph.
     *  \param A The list of nodes on the left side of the bipartite graph.
     *  \param B The list of nodes on the right side of the bipartite graph.
     *  \param rank An edge array. Each entry corresponds to the rank of an edge.
     *  \param capacity Node capacities.
     *  \param L A list of edges which after the algorithm will contain the computed matching.
     *           Undefined if no popular matching exists.
     *  \return True if the matching is popular, false otherwise.
     *  \pre Capacities of the left side of the graph have to be 1, capacities of the right
     *       side have to be positive.
     *  \pre The graph must be bipartite
     *  \ingroup popular
     */
    bool BI_POPULAR_CAPACITATED_MATCHING( 
            const graph& G, 
            const list<node>& A, 
            const list<node>& B, 
            const edge_array<int>& rank, 
            const node_array<int>& capacity,
            list<edge>& L );

    /*! \brief Compute the unpopularity margin of a matching. 
     *  
     *  See McCutchen 2007.
     *
     *  This function might change the order of the graph edges in the adjacency lists. 
     *
     *  \param G A bipartite graph.
     *  \param A The list of nodes on the left side of the bipartite graph.
     *  \param B The list of nodes on the right side of the bipartite graph.
     *  \param rank An edge array. Each entry corresponds to the rank of an edge.
     *  \param M The matching to test as a list of edges.
     *  \pre   All edges are directed from A to B.
     *  \return The unpopularity margin
     *  \ingroup popular
     */
    int BI_UNPOPULARITY_MARGIN( graph& G,
	    const list<node>& A,  const list<node>& B,
	    const edge_array<int>& rank,
	    const list<edge>& M );


}

#endif  // LEP_POPULAR_H

/* ex: set ts=4 sw=4 sts=4 et: */


