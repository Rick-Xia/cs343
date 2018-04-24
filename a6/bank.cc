#include "bank.h"

Bank::Bank( unsigned int numStudent ) : numStudent(numStudent) {
    Bank::accounts = new unsigned int[ numStudent ]();
    Bank::wthdBench = new uCondition[ numStudent ];
}

Bank::~Bank() { delete Bank::accounts; delete [] Bank::wthdBench; }

void Bank::deposit( unsigned int id, unsigned int amount ) {
    Bank::accounts[id] += amount;
    if ( !Bank::wthdBench[id].empty() ) Bank::wthdBench[id].signal();
}

void Bank::withdraw( unsigned int id, unsigned int amount ) {
    while ( Bank::accounts[id] < amount ) { wthdBench[id].wait(); }
    Bank::accounts[id] -= amount;
}
