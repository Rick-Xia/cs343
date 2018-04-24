#ifndef __GROUPOFF_H__
#define __GROUPOFF_H__

#include "watCard.h"
#include "MPRNG.h"

_Monitor Printer;
extern MPRNG mprng;

_Task Groupoff {
    struct Card {
        WATCard::FWATCard result;
        Card();
    };
    Printer *printer;
    unsigned int numStudents, sodaCost, groupoffDelay;
    unsigned int numWaitingCard, numCardGiven;
    Card** cardList;
    WATCard** distributed;

    void main();
  public:
    Groupoff( Printer &prt, unsigned int numStudents, unsigned int sodaCost, unsigned int groupoffDelay );
    ~Groupoff();
    WATCard::FWATCard giftCard();
};

#endif
