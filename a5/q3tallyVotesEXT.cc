#include "q3tallyVotes.h"
#include "q3printer.h"
#include "q3voter.h"
#include <algorithm>
using namespace std;

TallyVotes::TallyVotes( unsigned int group, Printer & printer ): group(group), numWaitingVoters(0), printer(&printer) {}

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

TallyVotes::Tour TallyVotes::vote( unsigned int id, Ballot ballot ) {
    TallyVotes::countBallot( ballot );
    TallyVotes::printer->print( id, Voter::States::Vote, ballot );

    TallyVotes::numWaitingVoters += 1;
    if ( TallyVotes::numWaitingVoters < TallyVotes::group ) {           // Check if this voter is the last one to form a group
        TallyVotes::printer->print( id, Voter::States::Block, TallyVotes::numWaitingVoters ); // It is NOT, wait for more voters coming
        _Accept( vote );                                                // Waiting on the acceptor stack
        TallyVotes::numWaitingVoters -= 1;                              // Automatically wake up when the accepted voter returns
        TallyVotes::printer->print( id, Voter::States::Unblock, TallyVotes::numWaitingVoters );
    } else {
        TallyVotes::printer->print( id, Voter::States::Complete );                            // It is, print the complete message
        TallyVotes::numWaitingVoters -= 1;
    } // if

    TallyVotes::Tour tour = TallyVotes::countResult();

    if ( !TallyVotes::numWaitingVoters ) TallyVotes::resetResult();     // Reset voting result for the next group, if this voter is the last one in a group

    return tour;
} // TallyVotes::vote
