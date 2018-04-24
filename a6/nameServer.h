#ifndef __NAMESERVER_H__
#define __NAMESERVER_H__

_Monitor Printer;
_Task VendingMachine;

_Task NameServer {
    Printer *printer;
    unsigned int numVendingMachines, numStudents, numVMregistered;
    VendingMachine **list;
    unsigned int *stuMapping;

    void main();
  public:
    NameServer( Printer &prt, unsigned int numVendingMachines, unsigned int numStudents );
    ~NameServer();
    void VMregister( VendingMachine *vendingMachine );
    VendingMachine* getMachine( unsigned int id );
    VendingMachine** getMachineList();
};

#endif
