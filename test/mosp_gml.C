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
// There is also a free version of LEDA 6.0 or newer.
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

#include <iostream>
#include <stdio.h>
#include <sys/resource.h>
#include <LEP/mosp/RANK_MAX_MATCHING.h>
#include <LEP/mosp/POPULAR.h>

#ifdef LEDA_GE_V5
#include <LEDA/graph/graph_misc.h>
#include <LEDA/graph/gml_graph.h>
#include <LEDA/graph/edge_map.h>
#include <LEDA/core/h_array.h>
#else
#include <LEDA/graph_misc.h>
#include <LEDA/gml_graph.h>
#include <LEDA/edge_map.h>
#include <LEDA/h_array.h>
#endif

#if defined(LEDA_NAMESPACE)
using namespace leda;
#endif

// length
edge_map<int> rankmap;
node_map<bool> isApplicantMap;
node_map<int> capacityMap;

// read edge rank
// in the following form: rank number
bool get_edge_rank( const gml_object* gobj, graph* G, edge e ) 
{
    int w = gobj->get_int();
    rankmap[ e ]= w;
    return true;
}

// read partition
// in the following form: partition 0
//                        partition 1
bool get_node_partition( const gml_object* gobj, graph* G, node v ) 
{ 
    int w = gobj->get_int();
    isApplicantMap[ v ] = (w == 0);
    return true;
}

bool get_node_capacity( const gml_object* gobj, graph* G, node v )
{
    int w = gobj->get_int();
    capacityMap[ v ] = w;
    return true;
}

void print_usage(const char * program)
{
    std::cout << "Usage: "<< program <<" [-r] [-p] [-u] [-c] [-f] [-m] [-o] [-t] [-v] [-h]" << std::endl;
    std::cout << "Read a GML graph from standard input and compute a matching with one sided preferences." << std::endl;
    std::cout << std::endl;
    std::cout << "-r" << std::endl;
    std::cout << "         Compute a rank-maximal matching." << std::endl;
    std::cout << "-a num" << std::endl;
    std::cout << "         If num = 1 then use the combinatorial algorithm." << std::endl;
    std::cout << "         If num = 2 then use the implicit reduction to maximum weight matching." << std::endl;
    std::cout << "         If num = 3 then use the explicit reduction to maximum weight matching." << std::endl;
    std::cout << "-p" << std::endl;
    std::cout << "         Try to find a popular matching." << std::endl;
    std::cout << "-u" << std::endl;
    std::cout << "         Try to find a matching which is not very unpopular." << std::endl;
    std::cout << "         This algorithm may compute an arbitrarily bad matching." << std::endl;
    std::cout << "-c" << std::endl;
    std::cout << "         Allow an instance with capacities on the right side." << std::endl;
    std::cout << "-f" << std::endl;
    std::cout << "         Print the unpopularity factor of the computed matching." << std::endl;
    std::cout << "-m" << std::endl;
    std::cout << "         Print the unpopularity margin of the computed matching." << std::endl;
    std::cout << "-o" << std::endl;
    std::cout << "         Print the computed matching in the standard output." << std::endl;
    std::cout << "-t" << std::endl;
    std::cout << "         Print the time taken to compute the matching." << std::endl;
    std::cout << "-v" << std::endl;
    std::cout << "         Verbose output." << std::endl;
    std::cout << "-h" << std::endl;
    std::cout << "         Display this message." << std::endl;
    std::cout << std::endl;
    std::cout << "Report bugs to <dimitrios.michail@gmail.com>." << std::endl;
}

int main(int argc, char* argv[]) {

    bool rankmaximal = true;
    bool popular = false;
    bool unpopular = false;
    bool capacitated = false;
    bool printfactor = false;
    bool printmargin = false;
    bool printmatching = false;
    bool printtime = true;
    bool verbose = false;
    char rmm_algo = '1';
    int c;

    opterr = 0;

    while ((c = getopt (argc, argv, "rpufmohtvca:")) != -1)
        switch (c)
        {
            case 'r':
                rankmaximal = true; 
                popular = false;
                unpopular = false;
                break;
            case 'p':
                rankmaximal = false;
                popular = true;
                unpopular = false;
                break;
            case 'u':
                rankmaximal = false;
                popular = false;
                unpopular = true; 
                break;
            case 'c':
                capacitated = !capacitated;
                break;
            case 'f':
                printfactor = !printfactor;
                break;
            case 'm':
                printmargin = !printmargin;
                break;
            case 'o':
                printmatching = !printmatching;
                break;
            case 't':
                printtime = !printtime;
                break;
            case 'v':
                verbose = !verbose;
                break;
            case 'a': // which algo for rmm
                rmm_algo = optarg[0];
                if ( rmm_algo < '1' || rmm_algo > '3' ) 
                    rmm_algo = '1';
                break;
            case 'h':
            default:
                print_usage( argv[0] );
                exit( EXIT_SUCCESS );
        }

    // initialize Graph
    graph G;
    rankmap.init( G );
    isApplicantMap.init( G );
    capacityMap.init( G );

    // create parser and read from standard input
    gml_graph parser( G );
    parser.add_edge_rule( get_edge_rank, gml_int, "rank" );
    parser.add_node_rule( get_node_partition, gml_int, "partition" );
    parser.add_node_rule( get_node_capacity, gml_int, "capacity" );

    if ( parser.parse( std::cin ) == false ) { 
        std::cerr << "Error parsing GML file, aborting.." << std::endl;
        abort();
    }

    // copy from map to edge array
    edge e;
    edge_array<int> rank( G, 1 );
    forall_edges( e, G )
        rank[ e ] = rankmap [ e ];
    G.sort_edges( rank );

    // copy capacities from map to node array
    node v;
    node_array<int> capacity( G, 1 );
    forall_nodes( v, G )
        capacity[ v ] = capacityMap[ v ];

    // fix node partition
    leda::list< node > A,B;
    forall_nodes( v, G )
        if ( isApplicantMap[v] )
            A.push(v);
        else 
            B.push(v);

    // direct edges from A to B
    forall( v, A )
        forall_in_edges( e, v )
            G.rev_edge( e );

    float T = 0.0, Ttotal = 0.0;

    leda::list< edge > L;
    if ( rankmaximal ) 
    {
        if ( capacitated ) 
        { 
            if ( verbose )
                std::cout << "Computing capacitated rank-maximal matching.." << std::endl;
            T = leda::used_time(); // start time
            L = mosp::BI_RANK_MAX_CAPACITATED_MATCHING( G, A, B, capacity, rank );
            Ttotal = used_time( T ); // finish time
        }
        else 
        {
            if ( verbose )
                std::cout << "Computing rank-maximal matching.." << std::endl;
            switch( rmm_algo ) 
            { 
                case '2': 
                    if ( verbose ) 
                        std::cout << "Using implicit reduction to MWM.." << std::endl;
                    T = leda::used_time(); // start time
                    L = mosp::DBI_RANK_MAX_MATCHING_MWMR( G, rank );
                    Ttotal = used_time( T ); // finish time
                    break;  
                case '3':
                    if ( verbose ) 
                        std::cout << "Using explicit reduction to MWM.." << std::endl;
                    T = leda::used_time(); // start time
                    L = mosp::BI_RANK_MAX_MATCHING_MWMR( G, rank );
                    Ttotal = used_time( T ); // finish time
                    break;  
                default:
                    if ( verbose ) 
                        std::cout << "Using combinatorial algorithm.." << std::endl;
                    T = leda::used_time(); // start time
                    L = mosp::BI_RANK_MAX_MATCHING( G, rank );
                    Ttotal = used_time( T ); // finish time
                    break;  
            };
        }
        G.sort_edges( rank );
    }
    else if ( popular ) 
    {
        if ( capacitated ) 
        {
            if (verbose)
                std::cout << "Computing capacitated popular matching.." << std::endl;
            T = leda::used_time(); // start time
            mosp::BI_POPULAR_CAPACITATED_MATCHING( G, A, B, rank, capacity, L );
            Ttotal = used_time( T ); // finish time
        }
        else { 
             if (verbose)
                std::cout << "Computing popular matching.." << std::endl;
            T = leda::used_time(); // start time
            mosp::BI_POPULAR_MATCHING( G, A, B, rank, L );
            Ttotal = used_time( T ); // finish time
        }
    }
    else
    {
        if (verbose)
            std::cout << "Computing not so unpopular matching.." << std::endl;
        int phase; 
        T = leda::used_time(); // start time
        mosp::BI_APPROX_POPULAR_MATCHING( G, A, B, rank, L, phase );
        Ttotal = used_time( T ); // finish time
    }

    if ( printfactor )
    {
        int factor;
        bool finite = mosp::BI_UNPOPULARITY_FACTOR( G, A, B, rank, L, factor );
        std::cout << "Unpopularity factor: "; 
        if ( finite ) 
            std::cout << factor;
        else
            std::cout << "oo";
        std::cout << std::endl;
    }

    if ( printmargin ) 
    {
        int margin; 
        margin = mosp::BI_UNPOPULARITY_MARGIN( G, A, B, rank, L );
        std::cout << "Unpopularity margin: " << margin << std::endl;

        // here G's edges are not sorted on rank anymore
    }

    if ( printmatching )
    {
        std::cout << "Matching: ";
        forall(e, L) 
        {
            std::cout << "(" << G.source(e)->id(); 
            std::cout << "," << G.target(e)->id() << ")";
            std::cout << " of rank " << rank[e];
            std::cout << std::endl;
        }
        std::cout << std::endl;
    }

    if ( printtime )
        std::cout << "time to compute: " << Ttotal << std::endl;

    return 0;
}

/* ex: set ts=4 sw=4 sts=4 et: */


