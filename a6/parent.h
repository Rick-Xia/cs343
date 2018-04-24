#ifndef __PARENT_H__
#define __PARENT_H__

#include "MPRNG.h"
_Monitor Printer;
_Monitor Bank;
extern MPRNG mprng;

_Task Parent {
    Printer *printer;
    Bank *bank;
    unsigned int numStudent, parentalDelay;

    void main();
  public:
    Parent( Printer &prt, Bank &bank, unsigned int numStudent, unsigned int parentalDelay );
    ~Parent();
};

#endif
