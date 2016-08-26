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
// Copyright (C) 2004-2009 Dimitrios Michail <dimitrios.michail@gmail.com>
//

#include <LEP/mosp/mosp.h>

#ifdef LEDA_GE_V5
#include <LEDA/graphics/graphwin.h>
#else
#include <LEDA/graphwin.h>
#endif // LEDA_GE_V5


#if defined(LEDA_NAMESPACE)
using leda::graph;
using leda::GRAPH;
using leda::GraphWin;
using leda::edge;
using leda::node;
#endif

GRAPH<leda::string,leda::string> G;
leda::edge_map<int> Gcost(G);

void run_and_display(GraphWin& gw) { 

    bool flush = gw.set_flush(false);

    gw.message("\\bf Computing Popular Matchings");

    leda::edge_array<int> cost(G);
    leda::edge e;
    forall_edges(e,G) cost[e] = Gcost[e];

    leda::list<node> A, B;
    if ( ! leda::Is_Bipartite(G, A, B) ) {
        gw.message("\\bf Graph is NOT Bipartite!");
    }

    // fix graph to comply with preconditions
    leda::Make_Simple( G );
    leda::Delete_Loops( G );
    node v;
    forall( v, A ) {
            forall_in_edges( e, v )
                G.rev_edge( e );
        }
    G.sort_edges( cost );
    gw.update_graph();


    leda::list<edge> C;
    int phases;
    bool is_popular = mosp::BI_APPROX_POPULAR_MATCHING( G, A, B, cost, C, phases );

    forall_edges(e,G) {
        gw.set_color( e, leda::black );
        gw.set_label(e, leda::string("%d",cost[e]));
    }

    leda::list_item lq;
    forall_items(lq, C){
        gw.set_color( C.contents(lq), leda::red );
    }

    // check correctness
    if ( ! mosp::DEBUG_is_valid_matching( G, C ) ) { 
        gw.message("\\bf ERROR, not a matching!" );
    }
    else { 
	if ( is_popular )
	    gw.message("\\bf Matching is popular!" );
	else { 
	    int factor;
	    bool notinf = mosp::BI_UNPOPULARITY_FACTOR( G, A, B, cost, C, factor );
	    if ( notinf ) {
		leda::string s( "\\bf Matching has unpopularity factor %d !", factor );
		gw.message( s );
	    }
	    else 
		gw.message( "\\bf Matching has unpopularity factor INFINITY!" );
	} 
    }
    

    gw.set_flush(flush);
    gw.redraw();
}

void new_node_handler(GraphWin& gw, node) {}

void new_edge_handler(GraphWin& gw, edge e) {
        if ( ! leda::Is_Bipartite(G) ) {
                gw.message("\\bf Cannot add edge, graph will NOT be bipartite!");
                gw.del_edge( e );
                return;
        }
        Gcost[e] = leda::rand_int(1,10);
        gw.set_slider_value(e,Gcost[e]/10.0,1);
        run_and_display(gw);
}

void del_edge_handler(GraphWin& gw) { 
        run_and_display(gw);
}

void del_node_handler(GraphWin& gw) { 
        run_and_display(gw);
}

void init_graph_handler( GraphWin &gw ) { 
    edge e;
    forall_edges(e,G) {
        if ( Gcost[e] == 0 ) {
            Gcost[e] = leda::rand_int(1, 10);
        }
        gw.set_slider_value(e,Gcost[e]/10.0,1);
    }
    run_and_display(gw);
}

// rank sliders

void start_cost_slider_handler(GraphWin& gw, edge, double)
{ 
        gw.message("\\bf\\red Change Edge Rank"); 
}

void cost_slider_handler(GraphWin& gw, edge e, double f) 
{ 
        Gcost[e] = int(10*f);
        if( Gcost[e] == 0 ) Gcost[e] = 1;
}

void end_cost_slider_handler(GraphWin& gw, edge, double)
{ 
        run_and_display(gw);
}

int main() 
{
    GraphWin gw(G,"GW_POPULAR_DEMO");

    gw.add_help_text("GW_POPULAR_DEMO");
    gw.display();
    gw.display_help_text("GW_POPULAR_DEMO");

    //gw.set_action(A_LEFT | A_DRAG | A_EDGE , NULL);
    gw.win_init(0,200,0);

    // set handlers

    gw.set_init_graph_handler(init_graph_handler);

    gw.set_del_edge_handler(del_edge_handler);
    gw.set_del_node_handler(del_node_handler);
    gw.set_new_edge_handler(new_edge_handler);

    gw.set_start_edge_slider_handler(start_cost_slider_handler,1);
    gw.set_edge_slider_handler(cost_slider_handler,1);
    gw.set_end_edge_slider_handler(end_cost_slider_handler,1);
    gw.set_edge_slider_color(leda::blue,1);

    gw.set_directed( false );

    gw.edit();

    return 0;
}

/* ex: set ts=8 sw=4 sts=4 noet: */


