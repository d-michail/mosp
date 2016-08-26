
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


/*! \file graphcopy.h
 *  \brief A copy of a graph with a mapping to the original graph.
 *
 */

#ifndef GRAPH_COPY_H
#define GRAPH_COPY_H

#include <LEP/mosp/config.h>

#ifdef LEDA_GE_V5
#include <LEDA/core/list.h>
#include <LEDA/graph/graph.h>
#include <LEDA/graph/edge_map.h>
#include <LEDA/graph/edge_array.h>
#include <LEDA/graph/node_map.h>
#include <LEDA/graph/node_array.h>
#else
#include <LEDA/list.h>
#include <LEDA/graph.h>
#include <LEDA/edge_map.h>
#include <LEDA/edge_array.h>
#include <LEDA/node_map.h>
#include <LEDA/node_array.h>
#endif

// start our namespace
namespace mosp
{ 

#if defined(LEDA_NAMESPACE)
    using leda::graph;
    using leda::GRAPH;
    using leda::edge;
    using leda::edge_map;
    using leda::edge_array;
    using leda::list;
    using leda::node;
    using leda::node_map;
    using leda::node_array;
    using leda::error_handler;
#endif

    // TODO: make this use template specialization and traits
    //       so that it works for both GRAPH<V,E> and plain graph

    class graph_copy: public graph
    {
        public: 
            graph_copy( const graph& g ) : G(g) { 
                initialize();
            }
            ~graph_copy() {}

            // map from edge to original's graph edge
            edge map( const edge& e ) { 
                return e_map[ e ];
            }
            // map from node to original's graph node
            node map( const node& v ) { 
                return v_map[ v ];
            }
            // map from edge of original graph to this graph
            edge map_rev( const edge& e ) {
                return e_map_rev[ e ];
            }
            // map from node of original graph to this graph
            node map_rev( const node& v ) { 
                return v_map_rev[ v ];
            }

            template<typename T>
                void copy_edge_array( const edge_array<T>& in, edge_array<T>& out ) 
                {
                    out.init( *this, T() );

                    edge e;
                    forall_edges( e, G ) 
                        out[ map_rev( e ) ] = in[ e ];
                }

            template<typename T>
                void copy_node_array( const node_array<T>& in, node_array<T>& out ) 
                {
                    out.init( *this, T() );

                    node v;
                    forall_nodes( v, G ) 
                        out[ map_rev( v ) ] = in[ v ];
                }

            void copy_node_list( const list<node>& in, list<node>& out) { 
                out.clear();
                node v;
                forall( v, in )
                    out.append( map_rev( v ) );
            }

            void copy_edge_list( const list<edge>& in, list<edge>& out) { 
                out.clear();
                edge e;
                forall( e, in )
                    out.append( map_rev( e ) );
            }

        private: 
            void initialize() { 
                // initialize mappings
                e_map.init( *this );
                v_map.init( *this );
                e_map_rev.init( G );
                v_map_rev.init( G );

                // fill them out
                node v, u;
                forall_nodes( v, G ) { 
                    u = this->new_node();
                    v_map[ u ] = v;
                    v_map_rev[ v ] = u;
                }

                edge e, q;
                forall_edges( e, G ) { 
                    q = this->new_edge( v_map_rev[ leda::source(e) ], v_map_rev[ leda::target(e) ] );
                    e_map[ q ] = e;
                    e_map_rev[ e ] = q;
                }
            }

            // variables
            const graph& G;
            leda::edge_map<edge> e_map;
            leda::edge_map<edge> e_map_rev;
            leda::node_map<node> v_map;
            leda::node_map<node> v_map_rev;
    };

}

#endif  // GRAPH_COPY_H

/* ex: set ts=4 sw=4 sts=4 et: */
