#include "bottlingPlant.h"
#include "truck.h"
#include "printer.h"
#include "MPRNG.h"

extern MPRNG mprng;

BottlingPlant::BottlingPlant( Printer &prt, NameServer &nameServer, unsigned int numVendingMachines, unsigned int maxShippedPerFlavour, unsigned int maxStockPerFlavour, unsigned int timeBetweenShipments ) : printer(&prt), nameServer(&nameServer), numVendingMachines(numVendingMachines), maxShippedPerFlavour(maxShippedPerFlavour), maxStockPerFlavour(maxStockPerFlavour), timeBetweenShipments(timeBetweenShipments), closing(false) {
    BottlingPlant::stock = new unsigned int[ 4 ]();
}

BottlingPlant::~BottlingPlant() {
    delete [] BottlingPlant::stock;
}

void BottlingPlant::getShipment( unsigned int cargo[] ) {
    if ( BottlingPlant::closing ) _Throw Shutdown();
    for ( unsigned int i = 0; i < 4; ++i ) {
        cargo[ i ] = BottlingPlant::stock[ i ];
    }
}

void BottlingPlant::main() {
    BottlingPlant::printer->print( Printer::Kind::BottlingPlant, 'S' );
    Truck *truck = new Truck( *BottlingPlant::printer, *BottlingPlant::nameServer, *this, BottlingPlant::numVendingMachines, BottlingPlant::maxStockPerFlavour);
    unsigned int totalGenerated = 0;

    try {
        for ( ;; ) {
            yield( BottlingPlant::timeBetweenShipments );
            for ( unsigned int i = 0; i < 4; ++i ) {
                BottlingPlant::stock[ i ] = mprng( BottlingPlant::maxShippedPerFlavour );
                totalGenerated += BottlingPlant::stock[ i ];
            }
            BottlingPlant::printer->print( Printer::Kind::BottlingPlant, 'G', totalGenerated );

            _Accept( ~BottlingPlant ) {
                BottlingPlant::closing = true;
                _Accept( getShipment );
            } or _Accept( BottlingPlant::getShipment ) {
                BottlingPlant::printer->print( Printer::Kind::BottlingPlant, 'P' );
                totalGenerated = 0;
            }
        }
    } catch ( uMutexFailure::RendezvousFailure ) { delete truck; }

    BottlingPlant::printer->print( Printer::Kind::BottlingPlant, 'F' );
}
