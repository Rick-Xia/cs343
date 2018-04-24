#ifndef __BOTTLINGPLANT_H__
#define __BOTTLINGPLANT_H__

_Monitor Printer;
_Task NameServer;

_Task BottlingPlant {
    Printer *printer;
    NameServer *nameServer;
    unsigned int numVendingMachines, maxShippedPerFlavour, maxStockPerFlavour, timeBetweenShipments;
    bool closing;
    unsigned int *stock;

    void main();
  public:
    _Event Shutdown {};                       // shutdown plant
    BottlingPlant( Printer &prt, NameServer &nameServer, unsigned int numVendingMachines, unsigned int maxShippedPerFlavour, unsigned int maxStockPerFlavour, unsigned int timeBetweenShipments );
    ~BottlingPlant();
    void getShipment( unsigned int cargo[] );
};

#endif
