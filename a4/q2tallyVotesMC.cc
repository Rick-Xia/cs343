#include "q2tallyVotes.h"
#include "q2printer.h"
#include "q2voter.h"
#include <algorithm>
using namespace std;

TallyVotes::TallyVotes( unsigned int group, Printer & printer ) : group(group), printer(&printer) {}

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

    if ( TallyVotes::waitingVoter == 0 ) TallyVotes::resetResult();

    return tour;
} // TallyVotes::countResult

void TallyVotes::resetResult() {                                        // Helper that reset the partial result
    fill( voteResult, voteResult+3, 0 );
} // TallyVotes::resetResult

TallyVotes::Tour TallyVotes::vote( unsigned int id, Ballot ballot ) {
    TallyVotes::ownerLock.acquire();                                    // Entry protocol
    if ( TallyVotes::bargingFlag ) {                                    // Check if the voter is barging 
        TallyVotes::printer->print( id, Voter::States::Barging );
        TallyVotes::bargeLock.wait( ownerLock );                        // If barging, wait inside the bargeLock
        TallyVotes::bargingFlag = !TallyVotes::bargeLock.empty();       // Reset the barge flag
    } // if

    TallyVotes::countBallot( ballot );
    TallyVotes::printer->print( id, Voter::States::Vote, ballot );

    if ( TallyVotes::waitingVoter < TallyVotes::group - 1 ) {           // Check if this voter is the last one in a group
        TallyVotes::waitingVoter++;
        TallyVotes::printer->print( id, Voter::States::Block, TallyVotes::waitingVoter );       // It is NOT, going to wait for more voters coming
        if ( !TallyVotes::bargeLock.empty() ) {                         // Check if there are voters waiting inside the bargeLock
            TallyVotes::bargingFlag = true;
            TallyVotes::bargeLock.signal();
        } // if

        TallyVotes::voteWaitLock.wait( ownerLock );                     // Wait inside the voteWaitLock
        TallyVotes::waitingVoter--;

        TallyVotes::printer->print( id, Voter::States::Unblock, TallyVotes::waitingVoter );
    } else {                                                                                    // It is, will not wait for more voters
        TallyVotes::printer->print( id, Voter::States::Complete );
    } // if

    Tour tour = TallyVotes::countResult();

    if ( !TallyVotes::waitingVoter ) TallyVotes::resetResult();

    if ( !TallyVotes::voteWaitLock.empty() ) {                          // Check if there are voters waiting inside the voteWaitLock
        TallyVotes::bargingFlag = true;
        TallyVotes::voteWaitLock.signal();
    } else if ( !TallyVotes::bargeLock.empty() ) {                      // Check if there are voters waiting inside the bargeLock
        TallyVotes::bargingFlag = true;
        TallyVotes::bargeLock.signal();
    } else TallyVotes::bargingFlag = false;                             // No waiting voters at all, reset the barge flag

    TallyVotes::ownerLock.release();                                    // Exit protocol
    return tour;
} // TallyVotes::vote

