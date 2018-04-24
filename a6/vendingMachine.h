#ifndef __VENDINGMACHINE_H__
#define __VENDINGMACHINE_H__

_Monitor Printer;
_Task NameServer;
class WATCard;

_Task VendingMachine {
    Printer *printer;
    NameServer *nameServer;
    unsigned int id, sodaCost, maxStockPerFlavour;
    unsigned int *stockPerFlavour;

    void main();
  public:
    enum Flavours { Ginger, Citrus, Lilium, Zanthoxylum, numFlavours };// flavours of soda (YOU DEFINE)
    _Event Free {};                           // free, advertisement
    _Event Funds {};                          // insufficient funds
    _Event Stock {};                          // out of stock for particular flavour
    VendingMachine( Printer &prt, NameServer &nameServer, unsigned int id, unsigned int sodaCost, unsigned int maxStockPerFlavour );
    ~VendingMachine();
    void buy( Flavours flavour, WATCard &card );
    unsigned int *inventory();
    void restocked();
    _Nomutex unsigned int cost();
    _Nomutex unsigned int getId();
};

#endif
