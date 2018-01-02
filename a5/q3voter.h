#ifndef __Q3VOTER_H__
#define __Q3VOTER_H__

#include "MPRNG.h"
#include "q3tallyVotes.h"
_Monitor Printer;
extern MPRNG mprng;

_Task Voter {
    unsigned int id;
    TallyVotes *tallier;
    Printer *printer;

    // Choose ranking of picture tour, then relationship of statue to gift shop.
    TallyVotes::Ballot cast();
    void main();
  public:
    enum States { Start = 'S', Vote = 'V', Block = 'B', Unblock = 'U', Barging = 'b',
                   Complete = 'C', Finished = 'F' };
    Voter( unsigned int id, TallyVotes & voteTallier, Printer & printer );
};

#endif
