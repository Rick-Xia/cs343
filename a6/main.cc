#include "MPRNG.h"
#include "config.h"
#include "printer.h"
#include "bank.h"
#include "parent.h"
#include "watCardOffice.h"
#include "groupoff.h"
#include "nameServer.h"
#include "vendingMachine.h"
#include "bottlingPlant.h"
#include "student.h"
#include <string>
#include <iostream>
using namespace std;

MPRNG mprng;

void uMain::main() {
    int seed = getpid();             // default values
    string filename = "soda.config";
    ConfigParms cprams;

    try {
        switch( argc ) {
            case 3: seed = stoi( argv[2] ); if ( to_string(seed) != argv[2] ) throw 1;
                // FALL THROUGH
            case 2: filename = argv[1];
                // FALL THROUGH
            case 1: if ( seed > 0 ) break;
                // FALL THROUGH
            default: throw 1;
        } // switch
    } catch ( ... ) {
        cerr << "Usage: " << argv[0] << " [ config-file [ random-seed (> 0) ] ]" << endl;
        exit( EXIT_FAILURE );
    } // try

    processConfigFile( filename.c_str(), cprams );
    mprng.set_seed( seed );

    Printer printer( cprams.numStudents, cprams.numVendingMachines, cprams.numCouriers);
    Bank bank( cprams.numStudents );
    Parent parent( printer, bank, cprams.numStudents, cprams.parentalDelay);
    WATCardOffice office( printer, bank, cprams.numCouriers );
    Groupoff groupoff( printer, cprams.numStudents, cprams.sodaCost, cprams.groupoffDelay );
    NameServer nameServer( printer, cprams.numVendingMachines, cprams.numStudents );

    VendingMachine* VMachines[cprams.numVendingMachines];
    for ( unsigned int i = 0; i < cprams.numVendingMachines; ++i ) {
        VMachines[i] = new VendingMachine( printer, nameServer, i, cprams.sodaCost, cprams.maxStockPerFlavour);
    }

    {
        BottlingPlant plant( printer, nameServer, cprams.numVendingMachines, cprams.maxShippedPerFlavour, cprams.maxStockPerFlavour, cprams.timeBetweenShipments);

        Student *students[cprams.numStudents];
        for ( unsigned int i = 0; i < cprams.numStudents; ++i ) {
            students[i] = new Student( printer, nameServer, office, groupoff, i, cprams.maxPurchases);
        }

        for ( unsigned int i = 0; i < cprams.numStudents; ++i ) {
            delete students[i];
        }
    }

    for ( unsigned int i = 0; i < cprams.numVendingMachines; ++i ) {
        delete VMachines[i];
    }
}
