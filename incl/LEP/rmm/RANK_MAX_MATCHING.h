#line 443 "RANK_MAX_MATCHING.lw"
//---------------------------------------------------------------------
// File automatically generated from notangle from RANK_MAX_MATCHING.lw
// 
// mails and bugs: Dimitrios Michail <dimitris.michail@gmail.com>
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


#line 360 "RANK_MAX_MATCHING.lw"
#ifndef LEP_RANK_MAX_MATCHING_H
#define LEP_RANK_MAX_MATCHING_H

#include <LEDA/array.h>
#include <LEDA/graph.h>
#include <LEDA/list.h>
#include <LEDA/edge_array.h>
#include <LEDA/std/assert.h>

/*{\Mtext
  
  {\bf \large Rank-Maximal Matchings}\medskip

  \settowidth{\typewidth}{|int|}
  \addtolength{\typewidth}{\colsep}
  \computewidths
  }*/


leda::list<leda::edge> BI_RANK_MAX_MATCHING( leda::graph& G, 
                          const leda::edge_array<int>& rank );

/*{\Mfunc $G$ is a simple, loopfree, bipartite graph and $rank$ is a 
positive integer function on the edges of $G$.
The function computes a Rank-Maximal matching $M$ of $G$, that is a 
matching which uses the largest possible number of rank one edges, and 
subject to this constraint the largest possible number of rank two edges and so on.  \\
The matching is returned as a list of edges.  \\
The running time is $O(C \sqrt{n} m)$ where $C$ is the maximum rank of an edge used in the matching. \\
\precond $G$ is simple, loopfree and bipartite \\
\precond $rank$ is positive
}*/

leda::list<leda::edge> BI_RANK_MAX_MATCHING_MWMR( leda::graph& G, 
                const leda::edge_array<int>& rank );
/*{\Mfunc $G$ is a simple, loopfree, bipartite graph and $rank$ is 
a positive integer function on the edges of $G$.
The function computes a Rank-Maximal matching $M$ of $G$, that is a 
matching which uses the largest possible number of rank one edges, 
and subject to this the largest possible number of rank two edges
and so on. \\
The matching is returned as a list of edges. \\
This procedure solves the problem by reducing it to the maximum 
weight matching problem, and therefore the running time is 
$O( n^2 ( m + n \log n ))$. The second $n$ comes from the possibly
large cost of arithmetic since the reduction involves edge weights
which can be as large as $n^n$. The space requirement is $O(mn + n^2)$. \\
\precond $G$ is simple, loopfree and bipartite \\
\precond $rank$ is positive
}*/


leda::array<int> BI_RANK_MAX_MATCHING_PROFILE( const leda::graph& G,
                                   const leda::edge_array<int>& rank,
                                   const leda::list<leda::edge>& matching);

/*{\Mfunc $G$ is a graph and $rank$ is a positive integer function
on the edges of $G$. $matching$ is a list of edges belonging to the matching.
The function returns an array $ar$ of non-negative integers representing the 
number of edges of each rank belonging to the matching. This array is indexed
from $1$ to $n$, where $n$ is the number of vertices of $G$. \\
The running time is $O(n)$ where $n$ is the number of nodes of $G$. \\
Note that this procedure does not check whether the list of edges is really
a matching or not. \\
\precond $rank$ is positive
}*/

// A procedure to check whether a list of edges is 
// a matching, for debugging and testing purposes.
bool RMM_DEBUG_is_valid_matching( leda::graph &G, 
	const leda::list<leda::edge>& matching );


#endif  // LEP_RANK_MAX_MATCHING_H

