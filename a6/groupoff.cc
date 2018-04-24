#include "groupoff.h"
#include "printer.h"

Groupoff::Groupoff( Printer &prt, unsigned int numStudents, unsigned int sodaCost, unsigned int groupoffDelay ) : printer(&prt), numStudents(numStudents), sodaCost(sodaCost), groupoffDelay(groupoffDelay), numWaitingCard(0), numCardGiven(0) {
    Groupoff::cardList = new Card *[ numStudents ];
    Groupoff::distributed = new WATCard *[ numStudents ];
}

Groupoff::~Groupoff() {
    for ( unsigned int i = 0; i < numWaitingCard; ++i ) {
        delete cardList[ i ];
    }
    for ( unsigned int i = 0; i < numCardGiven; ++i ) {
        delete distributed[ i ];
    }
    delete [] cardList;
    delete [] distributed;
}

Groupoff::Card::Card() {}

WATCard::FWATCard Groupoff::giftCard() {
    Groupoff::Card *c = new Groupoff::Card();
    Groupoff::cardList[ Groupoff::numWaitingCard ] = c;
    return c->result;
}

void Groupoff::main() {
    Groupoff::printer->print( Printer::Kind::Groupoff, 'S' );
    for ( ;; ) {
      if ( numCardGiven == numStudents ) break;
        _Accept( ~Groupoff ) {
            break;
        } or _Accept( Groupoff::giftCard ) {
            Groupoff::numWaitingCard += 1;
        } _Else {
      if ( Groupoff::numWaitingCard == 0 ) continue;
            yield( Groupoff::groupoffDelay );

            WATCard *newGiftCard = new WATCard();
            newGiftCard->deposit( Groupoff::sodaCost );

            unsigned int id = mprng( Groupoff::numWaitingCard - 1 );
            Groupoff::Card *c = Groupoff::cardList[ id ];            
            c->result.delivery( newGiftCard );
            Groupoff::printer->print( Printer::Kind::Groupoff, 'D', Groupoff::sodaCost );

            distributed[ numCardGiven ] = newGiftCard;
            delete c;

            Groupoff::cardList[ id ] = Groupoff::cardList[ Groupoff::numWaitingCard - 1 ];
            Groupoff::numWaitingCard -= 1;
            numCardGiven += 1;
        }
    }
    Groupoff::printer->print( Printer::Kind::Groupoff, 'F' );
}
