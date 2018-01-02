#ifndef __Q3TALLYVOTES_H__
#define __Q3TALLYVOTES_H__

_Monitor Printer;

#if defined( IMPLTYPE_EXT )                  // external scheduling monitor solution
    // includes for this kind of vote-tallier
_Monitor TallyVotes {
  public:                                    // common interface
    TallyVotes( unsigned int group, Printer & printer );
    struct Ballot { unsigned int picture, statue, giftshop; };
    enum Tour { Picture = 'p', Statue = 's', GiftShop = 'g' };
    Tour vote( unsigned int id, Ballot ballot );
  private:
    // private declarations for this kind of vote-tallier

#elif defined( IMPLTYPE_INT )                // internal scheduling monitor solution
    // includes for this kind of vote-tallier
_Monitor TallyVotes {
    // private declarations for this kind of vote-tallier
  public:                                    // common interface
    TallyVotes( unsigned int group, Printer & printer );
    struct Ballot { unsigned int picture, statue, giftshop; };
    enum Tour { Picture = 'p', Statue = 's', GiftShop = 'g' };
    Tour vote( unsigned int id, Ballot ballot );
  private:
    uCondition voterBench;

#elif defined( IMPLTYPE_INTB )               // internal scheduling monitor solution with barging
    // includes for this kind of vote-tallier
_Monitor TallyVotes {
  public:                                    // common interface
    TallyVotes( unsigned int group, Printer & printer );
    struct Ballot { unsigned int picture, statue, giftshop; };
    enum Tour { Picture = 'p', Statue = 's', GiftShop = 'g' };
    Tour vote( unsigned int id, Ballot ballot );
  private:
    // private declarations for this kind of vote-tallier
    uCondition bench;                        // only one condition variable (you may change the variable name)
    unsigned int ticketCounter, curGroup;
    void wait();                             // barging version of wait
    void signalAll();                        // unblock all waiting tasks

#elif defined( IMPLTYPE_AUTO )               // automatic-signal monitor solution
    // includes for this kind of vote-tallier
    #include "AutomaticSignal.h"
_Monitor TallyVotes {
  public:                                    // common interface
    TallyVotes( unsigned int group, Printer & printer );
    struct Ballot { unsigned int picture, statue, giftshop; };
    enum Tour { Picture = 'p', Statue = 's', GiftShop = 'g' };
    Tour vote( unsigned int id, Ballot ballot );
  private:
    // private declarations for this kind of vote-tallier
    AUTOMATIC_SIGNAL;
    bool voteFinish;

#elif defined( IMPLTYPE_TASK )               // internal/external scheduling task solution
_Task TallyVotes {
  public:                                    // common interface
    TallyVotes( unsigned int group, Printer & printer );
    struct Ballot { unsigned int picture, statue, giftshop; };
    enum Tour { Picture = 'p', Statue = 's', GiftShop = 'g' };
    Tour vote( unsigned int id, Ballot ballot );
  protected:
    void main();
  private:
    // private declarations for this kind of vote-tallier
    unsigned int inGroupCnt, curId;
    Ballot curBallot;
    uCondition voterBench;


#else
    #error unsupported voter type

#endif
    // common declarations
    unsigned int group, numWaitingVoters;
    int voteResult[3] = { 0, 0, 0 };
    Printer *printer;

    void countBallot( TallyVotes::Ballot ballot );      // Helper that cast the vote
    void resetResult();                                 // Helper that reset the partial result
    TallyVotes::Tour countResult();                     // Helper that gets the final result
};

#endif
