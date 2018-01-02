#include "MPRNG.h"
#include "q2tallyVotes.h"
#include "q2printer.h"
#include <iostream>
using namespace std;

MPRNG mprng;

void uMain::main() {
    int voters = 6, groupSize = 3, seed = getpid();             // default values

    try {
        switch( argc ) {
            case 4: seed = stoi( argv[3] );
                // FALL THROUGH
            case 3: groupSize = stoi( argv[2] );
                // FALL THROUGH
            case 2: voters = stoi( argv[1] );
                // FALL THROUGH
            case 1: if ( seed > 0 && groupSize > 0 && voters > 0 && voters % groupSize == 0 ) break;
                // FALL THROUGH
            default: throw 1;
        } // switch
    } catch ( ... ) {
        cerr << "Usage: " << argv[0] << "  Voters (> 0 & V mod G = 0, default 6)  Group (> 0, default 3)  Seed (> 0)" << endl;
        exit( EXIT_FAILURE );
    } // try

    mprng.set_seed( seed );
    Printer printer( voters );
    TallyVotes tally( groupSize, printer );
    Voter *voterArray[ voters ];
    for ( int i = 0; i < voters; ++i ) {                        // Initilize voters
        voterArray[ i ] = new Voter( i, tally, printer );
    } // for

    for ( int j = 0; j < voters; ++j ) {                        // Wait all voters to finish
        delete voterArray[ j ];
    } // for
} // uMain::main

