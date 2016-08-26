#line 443 "RANK_MAX_MATCHING.lw"
//---------------------------------------------------------------------
// File automatically generated from notangle from RANK_MAX_MATCHING.lw
// 
// mails and bugs: Dimitrios Michail <dimitrios.michail@gmail.com>
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

#line 1362 "RANK_MAX_MATCHING.lw"
#include <LEDA/graphwin.h>
#include <LEP/rmm/RANK_MAX_MATCHING.h>

#if defined(LEDA_NAMESPACE)
using leda::graph;
using leda::GraphWin;
using leda::edge;
using leda::node;
#endif

graph G;
leda::edge_map<int> Gcost(G);

void run_and_display(GraphWin& gw) { 

bool flush = gw.set_flush(false);

gw.message("\\bf Computing Rank-Maximal Matching");

leda::edge_array<int> cost(G);
node v; edge e;
forall_edges(e,G) cost[e] = Gcost[e];

if ( ! leda::Is_Bipartite(G) ) {
	gw.message("\\bf Graph is NOT Bipartite!");
}

// fix graph to comply with preconditions
leda::Make_Simple( G );
leda::Delete_Loops( G );

leda::list<edge> C;
C = BI_RANK_MAX_MATCHING( G , cost );

forall_edges(e,G) {
	gw.set_color( e, leda::black );
	gw.set_label(e, leda::string("%d",cost[e]));
}

leda::list_item lq;
forall_items(lq, C){
	gw.set_color( C.contents(lq), leda::red );
}

// check correctness
if ( ! RMM_DEBUG_is_valid_matching( G, C ) ) { 
	gw.message("\\bf ERROR, not a matching!" );
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
		Gcost[e] = leda::rand_int(1, 10);
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
GraphWin gw(G,"Rank-Maximal");

gw.add_help_text("RANK_MAX_MATCHING");
gw.display();
gw.display_help_text("RANK_MAX_MATCHING");

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

gw.edit();

return 0;
}



