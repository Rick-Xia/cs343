#include "truck.h"
#include "bottlingPlant.h"
#include "vendingMachine.h"
#include "nameServer.h"
#include "printer.h"
#include <algorithm>

Truck::Truck( Printer &prt, NameServer &nameServer, BottlingPlant &plant, unsigned int numVendingMachines, unsigned int maxStockPerFlavour ) : printer(&prt), nameServer(&nameServer), plant(&plant), numVendingMachines(numVendingMachines), maxStockPerFlavour(maxStockPerFlavour) {
    Truck::cargo = new unsigned int[4]();
}

Truck::~Truck() {
    delete [] Truck::cargo;
}

unsigned int Truck::cargoAmount() {
    unsigned int total = 0;
    for ( unsigned int i = 0; i < 4; ++i ) {
        total += Truck::cargo[i];
    }
    return total;
}

void Truck::main() {
    Truck::printer->print( Printer::Kind::Truck, 'S' );
    VendingMachine **list = Truck::nameServer->getMachineList();
    VendingMachine *trgtMachine;
    unsigned int nextMachine = 0;
    unsigned int numRestocked = 0;

    try {
        for ( ;; ) {
            yield( mprng(1, 10) ); // get coffee
            Truck::plant->getShipment( Truck::cargo );
            Truck::printer->print( Printer::Kind::Truck, 'P', Truck::cargoAmount() );
            numRestocked = 0;

            for ( ;; ) { // shipping
                if ( Truck::cargoAmount() == 0 ) break; // run out of shipment
                if ( numRestocked >= Truck::numVendingMachines ) break; // completed a full cycle

                trgtMachine = list[ nextMachine ]; // get next machine to restock
                nextMachine = ( nextMachine + 1 ) % Truck::numVendingMachines; // pointer update
                Truck::printer->print( Printer::Kind::Truck, 'd', trgtMachine->getId(), Truck::cargoAmount() );

                unsigned int *stock = trgtMachine->inventory();
                unsigned int unfill = 0;
                for ( unsigned int i = 0; i < 4; ++i ) {
                    unsigned int amount = std::min( Truck::maxStockPerFlavour - stock[i],
                                                Truck::cargo[i] );
                    stock[i] += amount;
                    cargo[i] -= amount;
                    unfill += Truck::maxStockPerFlavour - stock[i];
                }
                if ( unfill > 0 ) Truck::printer->print( Printer::Kind::Truck, 'U', trgtMachine->getId(), unfill );
                Truck::printer->print( Printer::Kind::Truck, 'D', trgtMachine->getId(), Truck::cargoAmount() );
                trgtMachine->restocked();
                numRestocked += 1;
            }
        } // for
    } catch ( BottlingPlant::Shutdown & ) {}
    Truck::printer->print( Printer::Kind::Truck, 'F' );
}
