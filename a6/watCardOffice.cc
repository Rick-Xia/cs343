#include "watCardOffice.h"
#include "watCard.h"
#include "bank.h"
#include "printer.h"
#include <queue>

WATCardOffice::Job::Job( unsigned int sid, unsigned int amount ) : id(sid), amount(amount), card(NULL) {} // amount === 5?

WATCardOffice::Job::Job( unsigned int sid, unsigned int amount, WATCard *card ) : id(sid), amount(amount), card(card) {} // amount === 5 + sodaCost?

//***********************************************
// Implementation of Couerier
//***********************************************

WATCardOffice::Courier::Courier( Printer &prt, Bank &bank, WATCardOffice *offi, unsigned int id ) : id(id), printer(&prt), bank(&bank), office(offi) {}

void WATCardOffice::Courier::main() {
    Courier::printer->print( Printer::Kind::Courier, Courier::id, 'S' );
    for ( ;; ) {
        Job *j = Courier::office->requestWork();
      if ( j->id == 0 && j->amount == 0 ) break; 
        Courier::printer->print( Printer::Kind::Courier, Courier::id, 't', j->id, j->amount );

        WATCard *newCard;
        if ( j->card != NULL ) newCard = j->card;
        else newCard = new WATCard();

        unsigned int amount = j->amount;
        Courier::bank->withdraw( j->id, amount );
        newCard->deposit( amount );

        if ( mprng( 5 ) == 0 ) { // card lost
            Courier::printer->print( Printer::Kind::Courier, Courier::id, 'L', j->id );
            j->result.exception( new WATCardOffice::Lost );
            delete newCard;
        } else {
            j->result.delivery( newCard );
            Courier::printer->print( Printer::Kind::Courier, Courier::id, 'T', j->id, j->amount );
        }
        delete j; // job satisfied
    }

    Courier::printer->print( Printer::Kind::Courier, Courier::id, 'F' );
}

//***********************************************
// Impletation of WATCardOffice
//***********************************************

WATCardOffice::WATCardOffice( Printer &prt, Bank &bank, unsigned int numCouriers ) : printer(&prt), bank(&bank), numCouriers(numCouriers) {
    WATCardOffice::courierPool = new Courier*[numCouriers];
    for ( unsigned int i = 0; i < WATCardOffice::numCouriers; ++i ) {
        WATCardOffice::courierPool[i] = new WATCardOffice::Courier( *WATCardOffice::printer, *WATCardOffice::bank, this, i );
    }
}

WATCardOffice::~WATCardOffice() {
    for ( unsigned int i = 0; i < WATCardOffice::numCouriers; ++i ) {
        delete WATCardOffice::courierPool[i];
    }
    delete [] WATCardOffice::courierPool;
}

WATCard::FWATCard WATCardOffice::create( unsigned int sid, unsigned int amount ) {
    WATCardOffice::Job *j = new Job( sid, amount );
    WATCardOffice::requests.push( j );
    WATCardOffice::printer->print( Printer::Kind::WATCardOffice, 'C', sid, amount );
    return j->result;
}

WATCard::FWATCard WATCardOffice::transfer( unsigned int sid, unsigned int amount, WATCard *card ) {
    WATCardOffice::Job *j = new Job( sid, amount, card );
    WATCardOffice::requests.push( j );
    WATCardOffice::printer->print( Printer::Kind::WATCardOffice, 'T', sid, amount );
    return j->result;
}

WATCardOffice::Job *WATCardOffice::requestWork() {
    return WATCardOffice::requests.front();
}

void WATCardOffice::main() {
    WATCardOffice::printer->print( Printer::Kind::WATCardOffice, 'S' );
    for ( ;; ) {
        try {
            _Accept( ~WATCardOffice ) {
                Job *j = new Job( 0, 0 );
                WATCardOffice::requests.push( j );
                for ( unsigned int i = 0; i < WATCardOffice::numCouriers; ++i ) {
                    _Accept( WATCardOffice::requestWork );
                }
                WATCardOffice::requests.pop();
                delete j;
                break;
            } or _Accept( WATCardOffice::create, WATCardOffice::transfer ) {
            } or _When( !WATCardOffice::requests.empty() ) _Accept( WATCardOffice::requestWork ) {
                WATCardOffice::requests.pop();
                WATCardOffice::printer->print( Printer::Kind::WATCardOffice, 'W' );
            }
        } catch ( uMutexFailure::RendezvousFailure ) {}
    }
    WATCardOffice::printer->print( Printer::Kind::WATCardOffice, 'F' );
}
