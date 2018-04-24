#ifndef __BANK_H__
#define __BANK_H__

_Monitor Bank {
    unsigned int numStudent;
    unsigned int *accounts;
    uCondition *wthdBench; // Can I do this?
  public:
    Bank( unsigned int numStudent );
    ~Bank();
    void deposit( unsigned int id, unsigned int amount );
    void withdraw( unsigned int id, unsigned int amount );
};

#endif
