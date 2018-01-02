#include "q2tallyVotes.h"
#include "q2printer.h"
#include "q2voter.h"
#include <algorithm>
using namespace std;

TallyVotes::TallyVotes( unsigned int group, Printer & printer ) : uBarrier(group), group(group), printer(&printer) {}

void TallyVotes::countBallot( Ballot ballot ) {                                 // Helper that cast the vote
    TallyVotes::voteResult[0] += ballot.giftshop;
    TallyVotes::voteResult[1] += ballot.picture;
    TallyVotes::voteResult[2] += ballot.statue;
} // TallyVotes::Ballot

TallyVotes::Tour TallyVotes::countResult() {                                    // Helper that looks for the tour having the highest vote
    int i = 0, max = 0;
    for ( int j = 0; j < 3; ++j ) {
        if ( max >= TallyVotes::voteResult[ j ] ) continue;
        max = voteResult[ j ];
        i = j;
    } // for

    return ( i == 0 )? Tour::GiftShop : ( ( i == 1 )? Tour::Picture : Tour::Statue );
} // TallyVotes::countResult

void TallyVotes::resetResult() {                                                // Helper that reset the partial voting result
    fill( voteResult, voteResult+3, 0 );
} // TallyVotes::resetResult

void TallyVotes::block( unsigned int id ) {                                     // Overriding uBarrier::block(), basically adding printing part
    if ( uBarrier::waiters() < TallyVotes::group - 1 ) {                       // If this voter is not the last one in a group
        TallyVotes::printer->print( id, Voter::States::Block, uBarrier::waiters() + 1 );
        uBarrier::block();
        TallyVotes::printer->print( id, Voter::States::Unblock, uBarrier::waiters() );
    } else {                                                                    // If this voter is the last one in a group
        TallyVotes::printer->print( id, Voter::States::Complete );
        uBarrier::block();
    } // if
} // TallyVotes::block

TallyVotes::Tour TallyVotes::vote( unsigned int id, Ballot ballot ) {
    if ( !uBarrier::waiters() ) TallyVotes::resetResult();

    TallyVotes::countBallot( ballot );                                          // Cast the vote
    TallyVotes::printer->print( id, Voter::States::Vote, ballot );

    TallyVotes::block( id );                                                    // Wait for more voters, or wake up all waiting voters

    Tour tour = TallyVotes::countResult();                                      // Get the final tour result

    if ( !uBarrier::waiters() ) TallyVotes::resetResult();                      // If there are no more voters waiting, reset the partial voting result

    return tour;
} // TallyVotes::vote

