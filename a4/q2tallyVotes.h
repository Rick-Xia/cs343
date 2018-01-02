#ifndef __Q2TALLYVOTES_H__
#define __Q2TALLYVOTES_H__

_Task Voter;
_Monitor Printer;

#if defined( IMPLTYPE_MC )              // mutex/condition solution
class TallyVotes {
  public:                               // common interface
    TallyVotes( unsigned int group, Printer & printer );
    struct Ballot { unsigned int picture, statue, giftshop; };
    enum Tour { Picture = 'p', Statue = 's', GiftShop = 'g' };
    Tour vote( unsigned int id, Ballot ballot );
  private:
                                        // private declarations for this kind of vote-tallier
    uOwnerLock ownerLock;               // MutexLock for whole voting process
    uCondLock voteWaitLock;             // CondLock for waiting for enough voters coming to the group
    uCondLock bargeLock;                // CondLock for barging avoidance
    bool bargingFlag = false;           // Flag variable for barging avoidance


#elif defined( IMPLTYPE_BAR )           // barrier solution
#include <uBarrier.h>                   // includes for this kind of vote-tallier
_Cormonitor TallyVotes : public uBarrier {
  public:                               // common interface
    TallyVotes( unsigned int group, Printer & printer );
    struct Ballot { unsigned int picture, statue, giftshop; };
    enum Tour { Picture = 'p', Statue = 's', GiftShop = 'g' };
    Tour vote( unsigned int id, Ballot ballot );
  private:                              // private declarations for this kind of vote-tallier
    void block( unsigned int id );      // Customized uBarrier::block()


#elif defined( IMPLTYPE_SEM )           // semaphore solution
#include <uSemaphore.h>                 // includes for this kind of vote-tallier
class TallyVotes {
  public:                               // common interface
    TallyVotes( unsigned int group, Printer & printer );
    struct Ballot { unsigned int picture, statue, giftshop; };
    enum Tour { Picture = 'p', Statue = 's', GiftShop = 'g' };
    Tour vote( unsigned int id, Ballot ballot );
  private:                              // private declarations for this kind of vote-tallier
    uSemaphore ownerSem;                // Semaphore for mutual exclusion
    uSemaphore bargeSem;                // Semaphore for barging prevention
    uSemaphore voteWaitSem;             // Semaphore for waiting for enough voters coming to the group

#else
    #error unsupported voter type
#endif

    // common declarations
    unsigned int group, waitingVoter = 0;
    int voteResult[3] = { 0, 0, 0 };    // Partial voting result, whose order is g, p, s
    Printer *printer;

    void countBallot( TallyVotes::Ballot ballot );      // Helper that cast the vote
    void resetResult();                                 // Helper that reset the partial result
    TallyVotes::Tour countResult();                     // Helper that gets the final result
};

#endif

