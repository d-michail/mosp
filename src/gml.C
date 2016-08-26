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

#ifdef LEDA_GE_V5
#include <LEDA/graph/graph.h>
#include <LEDA/system/assert.h>
#include <LEDA/core/array.h>
#else
#include <LEDA/graph.h>
#include <LEDA/std/assert.h>
#include <LEDA/array.h>
#endif // LEDA_GE_V5

namespace mosp
{

#if defined(LEDA_NAMESPACE)
    using leda::graph;
    using leda::node;
    using leda::edge;
#endif


void write_gml( const leda::graph& G, 
        const leda::list<leda::node>& A, 
        const leda::list<leda::node>& B,
        const leda::node_array<int>& capacity,
        const bool output_capacity,
        const leda::edge_array<int>& rank, 
        std::ostream& o )

{
    leda::node_array<int> par( G, 0 );
    leda::node v;
    forall( v, B )
        par[v] = 1;

    //leda::node_array<int> id( G, 0 );
    //int i = 0;
    //forall_nodes( v, G  )
        //id[ v ] = i++;

    o << "Creator \"MOSP LIBRARY\"" << std::endl;
    o << "graph [" << std::endl;
    o << "\tdirected 1" << std::endl;
    forall_nodes( v, G ) {
        o << "\tnode [" << std::endl;
        o << "\t\tid " << v->id() << std::endl;
        o << "\t\tpartition " << par[v] << std::endl;
        if ( output_capacity )
            o << "\t\tcapacity " << capacity[v] << std::endl;
        o << "\t]" << std::endl;
    }
    leda::edge e;
    forall_edges( e, G ) { 
        o << "\tedge [" << std::endl;
        o << "\t\tsource " << G.source(e)->id() << std::endl;
        o << "\t\ttarget " << G.target(e)->id() << std::endl;
        o << "\t\trank " << rank[ e ] << std::endl;
        o << "\t]" << std::endl;
    }
    o << "]" << std::endl;
}

void write_gml( const leda::graph& G, 
        const leda::list<leda::node>& A, 
        const leda::list<leda::node>& B,
        const leda::edge_array<int>& rank, 
        std::ostream& o 
        )
{
    leda::node_array<int> cap( G, 1 );
    write_gml( G, A, B, cap, false, rank, o );
}

void write_gml( const leda::graph& G, 
        const leda::list<leda::node>& A, 
        const leda::list<leda::node>& B,
        const leda::node_array<int>& capacity,
        const leda::edge_array<int>& rank, 
        std::ostream& o 
        )
{
    write_gml( G, A, B, capacity, true, rank, o );
}


    

} // end of namespace

/* ex: set ts=4 sw=4 sts=4 et: */
