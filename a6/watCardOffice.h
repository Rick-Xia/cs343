#ifndef __WATCARDOFFICE_H__
#define __WATCARDOFFICE_H__

#include "watCard.h"
#include "MPRNG.h"
#include <queue>

_Monitor Printer;
_Monitor Bank;
extern MPRNG mprng;

_Task WATCardOffice {
    struct Job {                              // marshalled arguments and return future
        unsigned int id, amount;
        WATCard *card;
        WATCard::FWATCard result;             // return future
        Job( unsigned int sid, unsigned int amount );
        Job( unsigned int sid, unsigned int amount, WATCard *card );
    };
    _Task Courier {                             // communicates with bank
        unsigned int id;
        Printer *printer;
        Bank *bank;
        WATCardOffice *office;

        void main();
      public:
        Courier( Printer &prt, Bank &bank, WATCardOffice *offi, unsigned int id );
    };

    Printer *printer;
    Bank *bank;
    unsigned int numCouriers;
    Courier **courierPool;
    std::queue<Job *> requests;

    void main();
  public:
    _Event Lost {};                           // lost WATCard
    WATCardOffice( Printer &prt, Bank &bank, unsigned int numCouriers );
    ~WATCardOffice();
    WATCard::FWATCard create( unsigned int sid, unsigned int amount );
    WATCard::FWATCard transfer( unsigned int sid, unsigned int amount, WATCard *card );
    Job *requestWork();
};

#endif
