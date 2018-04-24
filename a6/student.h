#ifndef __STUDENT_H__
#define __STUDENT_H__

#include "MPRNG.h"
class WATCard;
_Monitor Printer;
_Task NameServer;
_Task WATCardOffice;
_Task Groupoff;
extern MPRNG mprng;

_Task Student {
    Printer *printer;
    NameServer *nameServer;
    WATCardOffice *cardOffice;
    Groupoff *groupoff;
    unsigned int id, maxPurchases;

    void main();
  public:
    Student( Printer &prt, NameServer &nameServer, WATCardOffice &cardOffice, Groupoff &groupoff, unsigned int id, unsigned int maxPurchases );
    ~Student();
};

#endif
