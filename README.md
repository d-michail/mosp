Matchings with One Sided Preferences Library
--------------------------------------------

This page is the source repository for the package located at
<a href="https://d-michail.github.io/mosp/">d-michail.github.io/mosp</a>.

This library implements algorithms which compute different
matchings in bipartite graphs with one sided preferences.
At this point there are algorithms for two problems: 

a) rank-maximal matchings

   The library implements an algorithm to compute
   a rank-maximal matching in a bipartite graph.
   The running time is O(C \sqrt{n} m). Here n is 
   the number of vertices, m the number of edges 
   and C the largest rank used on the optimal solution.

   As a second algorithm, a straight-forward implementation
   of a straight-forward reduction to the maximum weight
   matching is provided with running time O(n^2(m + n \log n))
   and space O(n^2).

b) popular matchings

   The library contains an algorithm to compute a popular
   matching in a bipartite graph. The running time is O(\sqrt{n} m)
   using linear space.

This program can be freely used in an academic environment ONLY for research purposes, 
subject to the following restrictions:

1. The origin of this software must not be misrepresented; you must not claim that you wrote the original software. 
   If you use this software an acknowledgment in the product documentation is required.
2. Altered source versions must be plainly marked as such, and must not be misrepresented as being the original software.
3. This notice may not be removed or altered from any source distribution.

Any other use is strictly prohibited by the author, without an explicit permission.

This program can be freely used in an academic environment
ONLY for research purposes. Any other use is strictly
prohibited by the author, without an explicit permission.

Note that this program uses the LEDA library, which is
NOT free. For more details visit Algorithmic Solutions
at http://www.algorithmic-solutions.com/

```
!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!
! Any commercial use of this software is strictly !
! prohibited without explicit permission by the   !
! author.                                         !
!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!
```

This software is distributed in the hope that it will be useful,
but WITHOUT ANY WARRANTY; without even the implied warranty of
MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.

(C) Copyright 2004-2010, Dimitrios Michail <dimitrios.michail@gmail.com>

