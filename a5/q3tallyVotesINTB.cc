#include "q3tallyVotes.h"
#include "q3printer.h"
#include "q3voter.h"
#include <algorithm>
using namespace std;

TallyVotes::TallyVotes( unsigned int group, Printer & printer ): ticketCounter(0), curGroup(0), group(group), numWaitingVoters(0), printer(&printer) {}

void TallyVotes::countBallot( Ballot ballot ) {                         // Helper that cast the vote
    TallyVotes::voteResult[0] += ballot.giftshop;
    TallyVotes::voteResult[1] += ballot.picture;
    TallyVotes::voteResult[2] += ballot.statue;
} // TallyVotes::countBallot

TallyVotes::Tour TallyVotes::countResult() {                            // Helper that looks for the tour having the highest vote
    int i = 0, max = 0;
    for ( int j = 0; j < 3; ++j ) {
        if ( max >= TallyVotes::voteResult[ j ] ) continue;
        max = voteResult[ j ];
        i = j;
    } // for
    Tour tour = ( i == 0 )? Tour::GiftShop : ( ( i == 1 )? Tour::Picture : Tour::Statue );

    return tour;
} // TallyVotes::countResult

void TallyVotes::resetResult() {                                        // Helper that reset the partial result
    fill( voteResult, voteResult+3, 0 );
} // TallyVotes::resetResult

void TallyVotes::wait() {
    bench.wait();                            // wait until signalled
    while ( rand() % 2 == 0 ) {              // multiple bargers allowed
        _Accept( vote ) {                    // accept barging callers
        } _Else {                            // do not wait if no callers
        } // _Accept
    } // while
} // TallyVotes::wait

void TallyVotes::signalAll() {               // also useful
    while ( !bench.empty() ) bench.signal(); // drain the condition
} // TallyVotes::signalAll

TallyVotes::Tour TallyVotes::vote( unsigned int id, Ballot ballot ) {
    unsigned int ticketGroup = ( TallyVotes::ticketCounter++ ) / TallyVotes::group;     // Obtain a ticket for preventing barging
    while ( TallyVotes::curGroup != ticketGroup ) {                                     // Check if the ticket matches with the current voting group
        TallyVotes::printer->print( id, Voter::States::Barging );                       // Print barging message and wait if the ticket fail to match
        TallyVotes::wait();
    } // while

    TallyVotes::countBallot( ballot );
    TallyVotes::printer->print( id, Voter::States::Vote, ballot );

    TallyVotes::numWaitingVoters += 1;
    if ( TallyVotes::numWaitingVoters < TallyVotes::group ) {                           // Check if this voter is the last one to form a group
        TallyVotes::printer->print( id, Voter::States::Block, TallyVotes::numWaitingVoters );// It is NOT, wait for more voters coming
        TallyVotes::wait();
        TallyVotes::numWaitingVoters -= 1;
        TallyVotes::printer->print( id, Voter::States::Unblock, TallyVotes::numWaitingVoters );
    } else {
        TallyVotes::printer->print( id, Voter::States::Complete );                           // It is, print the complete message
        TallyVotes::numWaitingVoters -= 1;
        TallyVotes::signalAll();
    } // if

    TallyVotes::Tour tour = TallyVotes::countResult();
    if ( TallyVotes::numWaitingVoters == 0 ) {                                              // If this voter is the last one in a group, reset voting result, update ticket, and wake up voters who are waiting on barging bench
        TallyVotes::resetResult();
        TallyVotes::curGroup++;
        TallyVotes::signalAll();
    } // if

    return tour;
} // TallyVotes::vote
