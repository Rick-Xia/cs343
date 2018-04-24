#include "student.h"
#include "watCard.h"
#include "watCardOffice.h"
#include "groupoff.h"
#include "vendingMachine.h"
#include "printer.h"
#include "nameServer.h"
#include <uFuture.h>

#include <iostream>

Student::Student( Printer &prt, NameServer &nameServer, WATCardOffice &cardOffice, Groupoff &groupoff, unsigned int id, unsigned int maxPurchases ) : printer(&prt), nameServer(&nameServer), cardOffice(&cardOffice), groupoff(&groupoff), id(id), maxPurchases(maxPurchases) {}

Student::~Student() {}

void Student::main() {
    unsigned int toPurchase = mprng( 1, Student::maxPurchases );
    unsigned int flavour = mprng( 3 );
    Student::printer->print( Printer::Kind::Student, Student::id, 'S', flavour, toPurchase );

    WATCard::FWATCard myWatCard = Student::cardOffice->create( Student::id, 5 );
    WATCard::FWATCard giftCard = Student::groupoff->giftCard();

    VendingMachine *machine = Student::nameServer->getMachine( Student::id );
    Student::printer->print( Printer::Kind::Student, Student::id, 'V', machine->getId() );

    unsigned int sodaPurchased = 0;
    for ( ;; ) {
      if ( sodaPurchased == toPurchase ) break;
        yield( mprng(1, 10) );

        bool realCard = false;
        while ( !realCard ) {
            try {
                _Select( myWatCard || giftCard );
                if ( myWatCard.available() ) myWatCard();
                realCard = true;
            } _Catch ( WATCardOffice::Lost & ) {
                myWatCard.reset();
                Student::printer->print( Printer::Kind::Student, Student::id, 'L' );
                myWatCard = Student::cardOffice->create( Student::id, 5 );
            }
        }

        if ( giftCard.available() ) {
            try {
                machine->buy( (VendingMachine::Flavours)flavour, *giftCard() );
                Student::printer->print( Printer::Kind::Student, Student::id, 'G', flavour, giftCard()->getBalance() );
                giftCard.reset();
                sodaPurchased += 1;
            } catch ( VendingMachine::Free & ) {
                Student::printer->print( Printer::Kind::Student, Student::id, 'a', flavour, giftCard()->getBalance() );
                yield( 4 );
                sodaPurchased += 1;
            } catch ( VendingMachine::Stock & ) {
                machine = Student::nameServer->getMachine( Student::id );
                Student::printer->print( Printer::Kind::Student, Student::id, 'V', machine->getId() );
            }
        } else if ( myWatCard.available() ) {
            try {
                machine->buy( (VendingMachine::Flavours)flavour, *myWatCard() );
                Student::printer->print( Printer::Kind::Student, Student::id, 'B', flavour, myWatCard()->getBalance() );
                sodaPurchased += 1;
            } catch ( VendingMachine::Free & ) {
                Student::printer->print( Printer::Kind::Student, Student::id, 'A', flavour, myWatCard()->getBalance() );
                yield( 4 );
                sodaPurchased += 1;
            } catch ( VendingMachine::Funds & ) {
                myWatCard = Student::cardOffice->transfer( Student::id, 5 + machine->cost(), myWatCard() );
            } catch ( VendingMachine::Stock & ) {
                machine = Student::nameServer->getMachine( Student::id );
                Student::printer->print( Printer::Kind::Student, Student::id, 'V', machine->getId() );
            }
        } else {
            std::osacquire( std::cerr ) << "neither card available" << std::endl;
            exit( EXIT_FAILURE );
        } 
    }

    try {
        WATCard* garb = myWatCard();
        delete garb;
    } catch ( WATCardOffice::Lost & ) {}

    Student::printer->print( Printer::Kind::Student, Student::id, 'F' );
}
