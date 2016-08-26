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


#include <iostream>
#include <unistd.h>

#include <LEP/mosp/gml.h>
#include <LEP/mosp/generator.h>
#include <LEP/mosp/POPULAR.h>
#include <LEP/mosp/RANK_MAX_MATCHING.h>

#ifdef LEDA_GE_V5
#include <LEDA/core/random_source.h>
#include <LEDA/core/array.h>
#include <LEDA/core/d_array.h>
#else
#include <LEDA/random_source.h>
#include <LEDA/array.h>
#include <LEDA/d_array.h>
#endif

leda::random_source s;

// usage message
void usage( char *name )
{
    std::cout << "Usage: " << name << " options" << std::endl;
    std::cout << "options: " << std::endl;
    std::cout << "\t" << "-n number of vertices in the left side of the bipartite graph." << std::endl;
    std::cout << "\t" << "-m number of vertices in the right side of the bipartite graph." << std::endl;
    std::cout << "\t" << "   if not given then m = n ." << std::endl;
    std::cout << "\t" << "-p density of graph instance (probability of edge existance)." << std::endl;
    std::cout << "\t" << "-t probability that an edge is tied with each predecessor." << std::endl;
    std::cout << "\t" << "-l lambda." << std::endl;
    std::cout << "\t" << "-q right side vertex capacity for highly correlated and regional" << std::endl;
    std::cout << "\t" << "   instance generator (default is 1)." << std::endl;
    std::cout << "\t" << "-w Number of regions for regional instance generator." << std::endl;
    std::cout << "\t" << "-s seed for the random number generator." << std::endl;
    std::cout << "\t" << "-c generate a highly-correlated instance." << std::endl;
    std::cout << "\t" << "-f generate a fixed-size exponential instance." << std::endl;
    std::cout << "\t" << "-v generate a variable-size exponential instance." << std::endl;
    std::cout << "\t" << "-r generate a regional instance." << std::endl;
}

// main function
int main( int argc, char* argv[]) {

    int n = -1;         // number of vertices on left side
    int m = -1;	        // number of vertices on right side
    double p = -1.0; 	// density of graph instance (probability of edge existance)
    double t = -1.0;    // probability that an entry has a tie with the previous entry
    double l = -1.0;    // lambda parameter
    int q = 1;
    int regnum = -1;
    bool hc = false;
    bool fixeds = false;
    bool vars = false;
    bool regional = false;
    int seed = 32432532;

    if ( argc <= 1 ) { 
        usage(argv[0]);
        return 0;
    }

    int c;
    // use getopt to get parameters
    opterr = 0;
    while((c=getopt(argc,argv,"s:hcfrvn:m:p:t:l:q:w:"))!=-1) { 
        switch(c)
        {
            case 'h':
                usage( argv[0] );
                return -1;
            case 'n': 
                n = atoi( optarg );
                break;
            case 'm':
                m = atoi( optarg );
                break;
            case 'p': 
                p = atof( optarg );
                break;
            case 't':
                t = atof( optarg );
                break;
            case 'l':
                l = atof( optarg );
                break;
            case 'q': 
                q = atoi( optarg );
                if ( q < 1 ) 
                    q = 1;
                break;
            case 'w': 
                regnum = atoi( optarg );
                break;
            case 's': 
                seed = atoi( optarg );
                break;
            case 'c': 
                hc = true;
                fixeds = false;
                vars = false;
                regional = false;
                break;
            case 'f': 
                hc = false;
                fixeds = true;
                vars = false;
                regional = false;
                break;
            case 'v':
                hc = false;
                fixeds = false;
                vars = true;
                regional = false;
                break;
            case 'r': 
                hc = false;
                fixeds = false;
                vars = false;
                regional = true;
                break;
            case '?':
                std::cerr << "Problem with arguments." << std::endl;
                usage(argv[0]);
            default: 
                abort();
        }
    }

    if ( n < 1 ) { 
        std::cerr << "n has to be at least 1." << std::endl;
        usage(argv[0]);
        abort();
    }
    if ( m == -1 ) m = n;
    if ( m < 1 ) { 
        std::cerr << "m has to be at least 1." << std::endl;
        usage(argv[0]);
        abort();
    }
    if ( (hc || vars) && ( p < 0.0 || p > 1.0 ) ) { 
        std::cerr << "p must be a probability." << std::endl;
        usage(argv[0]);
        abort();
    }
    if ( (hc) && ( t < 0.0 || t > 1.0 ) ) { 
        std::cerr << "t must be a probability." << std::endl;
        usage(argv[0]);
        abort();
    }
    if ( (fixeds || vars || regional ) && l < 0.0 ) { 
        std::cerr << "lambda must be positive." << std::endl;
        usage(argv[0]);
        abort();
    }
    if ( regional && regnum <= 0 ) { 
        std::cerr << "number of regions must be positive (use -w option)." << std::endl;
        usage(argv[0]);
        abort();
    }
    s.set_seed( seed );

    mosp::StructuredInstanceGenerator *gen = 0; 
    
    if ( hc ) {
        gen = new mosp::HighlyCorrelatedInstanceGenerator( n, m, p, t, q, seed );
    }
    else if ( fixeds ) { 
        gen = new mosp::FSExponentialInstanceGenerator( n, m, l, seed );
    }
    else if ( vars ) { 
        gen = new mosp::VSExponentialInstanceGenerator( n, m, p, l, seed );
    }
    else if ( regional ) { 
        gen = new mosp::RegionalInstanceGenerator( n, m, q, regnum , l, seed );
    }
    else { 
        std::cerr << "You must choose an instance generator." << std::endl;
        usage(argv[0]);
        abort();
    }

    leda::graph G;
    leda::list<leda::node> A,B;
    leda::edge_map<int> rank(G);
    leda::node_map<int> capacity(G);

    // generate graph
    gen->GenerateGraph( G, A, B, capacity, rank );

    //G.sort_edges( rank );

    mosp::write_gml( G, A, B, capacity, rank, std::cout );

    // delete instance generator
    delete gen;

    return 0;
}

/* ex: set ts=4 sw=4 sts=4 et autoindent smartindent : */


