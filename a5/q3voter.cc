#include "q3voter.h"
#include "q3printer.h"
#include "q3tallyVotes.h"

Voter::Voter( unsigned int id, TallyVotes & voteTallier, Printer & printer ) : id(id), tallier(&voteTallier), printer(&printer) {}

TallyVotes::Ballot Voter::cast() {              // cast 3-way vote
    static unsigned int voting[3][2][2] = { { {2,1}, {1,2} }, { {0,2}, {2,0} }, { {0,1}, {1,0} } };
    unsigned int picture = mprng( 2 ), statue = mprng( 1 );
    return (TallyVotes::Ballot){ picture, voting[picture][statue][0], voting[picture][statue][1] };
} // Voter::cast

void Voter::main() {
    int rest = mprng( 19 );                                             // Yield a random number of times
    yield( rest );

    Voter::printer->print( Voter::id, Voter::States::Start );           // Print start msg

    yield( 1 );                                                         // Yield once

    TallyVotes::Ballot ballot = Voter::cast();
    TallyVotes::Tour tour = Voter::tallier->vote( Voter::id, ballot );  // Vote

    yield( 1 );                                                         // Yield once

    Voter::printer->print( Voter::id, Voter::States::Finished, tour );  // Print finish msg
} // Voter::main
