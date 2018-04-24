#include "parent.h"
#include "bank.h"
#include "printer.h"

Parent::Parent( Printer &prt, Bank &bank, unsigned int numStudent, unsigned int parentalDelay ) : printer(&prt), bank(&bank), numStudent(numStudent), parentalDelay(parentalDelay) {}

Parent::~Parent() {
    Parent::printer->print( Printer::Kind::Parent, 'F' );
}

void Parent::main() {
    Parent::printer->print( Printer::Kind::Parent, 'S' );
    for ( ;; ) {
        _Accept( Parent::~Parent ) {
            break;
        } _Else {
            unsigned int gift = mprng( 1, 3 );
            unsigned int stu = mprng( Parent::numStudent - 1 );
            yield( Parent::parentalDelay );
            Parent::bank->deposit( stu, gift );
            Parent::printer->print( Printer::Kind::Parent, 'D', stu, gift );
        }
    }
}
