#include "nameServer.h"
#include "vendingMachine.h"
#include "printer.h"

NameServer::NameServer( Printer &prt, unsigned int numVendingMachines, unsigned int numStudents ) : printer(&prt), numVendingMachines(numVendingMachines), numStudents(numStudents), numVMregistered(0) {
    NameServer::list = new VendingMachine *[ numVendingMachines ];
    NameServer::stuMapping = new unsigned int[ numStudents ];
    for ( unsigned int i = 0; i < numStudents; ++i ) {
        NameServer::stuMapping[ i ] = i % numVendingMachines;
    }
}

NameServer::~NameServer() {
    delete [] NameServer::stuMapping;
    delete [] NameServer::list;
}

void NameServer::VMregister( VendingMachine *vendingMachine ) {
    NameServer::list[ NameServer::numVMregistered ] = vendingMachine;
}

VendingMachine *NameServer::getMachine( unsigned int id ) {
    unsigned int pos = NameServer::stuMapping[ id ];
    NameServer::stuMapping[ id ] = ( NameServer::stuMapping[ id ] + 1 ) % NameServer::numVendingMachines;
    NameServer::printer->print( Printer::Kind::NameServer, 'N', id, NameServer::list[ pos ]->getId() );
    return NameServer::list[ pos ];
}

VendingMachine **NameServer::getMachineList() {
    return NameServer::list;
}

void NameServer::main() {
    NameServer::printer->print( Printer::Kind::NameServer, 'S' );
    for ( ;; ) {
        _When( NameServer::numVMregistered >= NameServer::numVendingMachines ) _Accept( ~NameServer ) {
            break;
        } or _Accept( NameServer::VMregister ) {
            NameServer::printer->print( Printer::Kind::NameServer, 'R', NameServer::list[ numVMregistered ]->getId() );
            NameServer::numVMregistered += 1;
        } or _When( NameServer::numVMregistered >= NameServer::numVendingMachines ) _Accept( NameServer::getMachine ) {
        } or _When( NameServer::numVMregistered >= NameServer::numVendingMachines ) _Accept( NameServer::getMachineList ) {}
    }
    NameServer::printer->print( Printer::Kind::NameServer, 'F' );
}
