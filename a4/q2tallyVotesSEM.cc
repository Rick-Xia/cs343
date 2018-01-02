#include "q2tallyVotes.h"
#include "q2printer.h"
#include "q2voter.h"
#include <algorithm>
using namespace std;

TallyVotes::TallyVotes( unsigned int group, Printer & printer ) : voteWaitSem(0), group(group), printer(&printer) {}

void TallyVotes::countBallot( Ballot ballot ) {                 // Helper that cast the vote
    TallyVotes::voteResult[0] += ballot.giftshop;
    TallyVotes::voteResult[1] += ballot.picture;
    TallyVotes::voteResult[2] += ballot.statue;
} // TallyVotes::countBallot

TallyVotes::Tour TallyVotes::countResult() {                    // Helper that looks for the tour having the highest vote
    int i = 0, max = 0;
    for ( int j = 0; j < 3; ++j ) {
        if ( max >= TallyVotes::voteResult[ j ] ) continue;
        max = voteResult[ j ];
        i = j;
    } // for

    return ( i == 0 )? Tour::GiftShop : ( ( i == 1 )? Tour::Picture : Tour::Statue );
} // TallyVotes::countResult

void TallyVotes::resetResult() {                                // Helper that reset the partial voting reuslt
    fill( voteResult, voteResult+3, 0 );
} // TallyVotes::resetResult

TallyVotes::Tour TallyVotes::vote( unsigned int id, Ballot ballot ) {
    TallyVotes::bargeSem.P();                                   // BargeSemaphore for barging prevent, allowing only $group voters can access to the ownerSemaphore

    TallyVotes::ownerSem.P();                                   // Entry protocol

    TallyVotes::countBallot( ballot );
    TallyVotes::printer->print( id, Voter::States::Vote, ballot );

    if ( TallyVotes::waitingVoter < TallyVotes::group - 1 ) { // Check if this voter is the last one in a group
        TallyVotes::printer->print( id, Voter::States::Block, ++TallyVotes::waitingVoter );     // It is NOT, going to wait for more voters coming

        TallyVotes::bargeSem.V();                              // Release the bargeSemaphore, let one more voter wait at ownerSemaphore
        TallyVotes::voteWaitSem.P( TallyVotes::ownerSem );     // Wait inside the voteWaitSemaphore

        TallyVotes::printer->print( id, Voter::States::Unblock, --TallyVotes::waitingVoter );
    } else {
        TallyVotes::printer->print( id, Voter::States::Complete );                              // It is, will not wait for more voters
    } // if

    Tour tour = TallyVotes::countResult();

    if ( TallyVotes::waitingVoter == 0 ) TallyVotes::resetResult(); // Check if no more voters are waiting. If so, reset voting result for the next group

    if ( !TallyVotes::voteWaitSem.empty() )                   // Check if there are voters waiting inside the voteWaitSemaphore, pass the bargeSemaphore
        TallyVotes::voteWaitSem.V();
    else TallyVotes::bargeSem.V();                            // No waiting voters anymore, release the bargeSemaphore

    TallyVotes::ownerSem.V();                                 // Exit protocol
    return tour;
} // TallyVotes::vote

