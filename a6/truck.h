#ifndef __TRUCK_H__
#define __TRUCK_H__

#include "MPRNG.h"
_Monitor Printer;
_Task NameServer;
_Task BottlingPlant;
extern MPRNG mprng;

_Task Truck {
    Printer *printer;
    NameServer *nameServer;
    BottlingPlant *plant;
    unsigned int numVendingMachines, maxStockPerFlavour;
    unsigned int *cargo;
    unsigned int cargoAmount();

    void main();
  public:
    Truck( Printer &prt, NameServer &nameServer, BottlingPlant &plant, unsigned int numVendingMachines, unsigned int maxStockPerFlavour );
    ~Truck();
};

#endif
