#include "q3tallyVotes.h"
#include "q3printer.h"
#include "q3voter.h"
#include <algorithm>
using namespace std;

TallyVotes::TallyVotes( unsigned int group, Printer & printer ): inGroupCnt(0), group(group), numWaitingVoters(0), printer(&printer) {}

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
    TallyVotes::curBallot = ballot;                                     // Tell main() the voter's info
    TallyVotes::curId = id;

    TallyVotes::voterBench.wait( id );                                  // Wait, and keep the id in the "shadow queue"

    TallyVotes::Tour tour = TallyVotes::countResult();
    return tour;
} // TallyVotes::vote

void TallyVotes::main() {
    for ( ;; ) {
        _Accept( ~TallyVotes ) { break; }
        or _When ( TallyVotes::inGroupCnt < TallyVotes::group ) _Accept( TallyVotes::vote ) {
            TallyVotes::inGroupCnt += 1;

            TallyVotes::countBallot( curBallot );                       // Vote on behalf of voters
            TallyVotes::printer->print( curId, Voter::States::Vote, curBallot );
            TallyVotes::printer->print( curId, Voter::States::Block, ++TallyVotes::numWaitingVoters );

            if ( TallyVotes::inGroupCnt == TallyVotes::group ) {                // If a group is full
                TallyVotes::printer->print( curId, Voter::States::Complete );   // Print Complete message on behalf of the last voter
                while ( !TallyVotes::voterBench.empty() ) {                     // Wake up waiting voters
                    TallyVotes::curId = TallyVotes::voterBench.front();
                    TallyVotes::printer->print( curId, Voter::States::Unblock, --TallyVotes::numWaitingVoters );
                    TallyVotes::voterBench.signalBlock();
                } // while

                // Reach here only if all voters have returned
                TallyVotes::resetResult();                                      // Reset voting result for the next group
                TallyVotes::inGroupCnt = 0;
            } // if
        } // _When
    } // for
} // TallyVotes::main
