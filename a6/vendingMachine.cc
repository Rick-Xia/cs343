#include "vendingMachine.h"
#include "nameServer.h"
#include "printer.h"
#include "watCard.h"
#include "MPRNG.h"

#include <iostream>

extern MPRNG mprng;

VendingMachine::VendingMachine( Printer &prt, NameServer &nameServer, unsigned int id, unsigned int sodaCost, unsigned int maxStockPerFlavour ) : printer(&prt), nameServer(&nameServer), id(id), sodaCost(sodaCost), maxStockPerFlavour(maxStockPerFlavour) {
    VendingMachine::stockPerFlavour = new unsigned int[4]();
    VendingMachine::nameServer->VMregister( this );
}

VendingMachine::~VendingMachine() {
    delete [] VendingMachine::stockPerFlavour;
}

void VendingMachine::buy( Flavours flavour, WATCard &card ) {
    if ( VendingMachine::stockPerFlavour[ (unsigned int) flavour ] <= 0 ) _Throw VendingMachine::Stock();
    if ( card.getBalance() < VendingMachine::sodaCost ) _Throw VendingMachine::Funds();
    if ( mprng( 4 ) == 0 ) {
        VendingMachine::stockPerFlavour[ (unsigned int) flavour ]--;
        VendingMachine::printer->print( Printer::Kind::Vending, VendingMachine::id, 'B', (int)flavour, VendingMachine::stockPerFlavour[ (unsigned int) flavour ] );
        _Throw VendingMachine::Free();
    }

    card.withdraw( VendingMachine::sodaCost );
    VendingMachine::stockPerFlavour[ (unsigned int) flavour ]--;
    VendingMachine::printer->print( Printer::Kind::Vending, VendingMachine::id, 'B', (int)flavour, VendingMachine::stockPerFlavour[ (unsigned int) flavour ] );
}

unsigned int* VendingMachine::inventory() {
    return VendingMachine::stockPerFlavour;
}

void VendingMachine::restocked() {}

_Nomutex unsigned int VendingMachine::cost() {
    return VendingMachine::sodaCost;
}

_Nomutex unsigned int VendingMachine::getId() {
    return VendingMachine::id;
}

void VendingMachine::main() {
    VendingMachine::printer->print( Printer::Kind::Vending, VendingMachine::id, 'S', VendingMachine::sodaCost );
for ( ;; ) {
        try {
            _Accept( ~VendingMachine ) {
                break;
            } or _Accept( buy ) {
            } or _Accept( VendingMachine::inventory ) {
                VendingMachine::printer->print( Printer::Kind::Vending, VendingMachine::id, 'r' );
                _Accept( VendingMachine::restocked );
                VendingMachine::printer->print( Printer::Kind::Vending, VendingMachine::id, 'R' );
            }
        } catch ( uMutexFailure::RendezvousFailure ) {}
    }
    VendingMachine::printer->print( Printer::Kind::Vending, VendingMachine::id, 'F' );
}
