#ifndef __PRINTER_H__
#define __PRINTER_H__

#include <string>

_Monitor Printer {
  public:
    enum Kind { Parent, Groupoff, WATCardOffice, NameServer, Truck, BottlingPlant, Student, Vending, Courier };
    Printer( unsigned int numStudents, unsigned int numVendingMachines, unsigned int numCouriers );
    ~Printer();
    void print( Kind kind, char state );
    void print( Kind kind, char state, int value1 );
    void print( Kind kind, char state, int value1, int value2 );
    void print( Kind kind, unsigned int lid, char state );
    void print( Kind kind, unsigned int lid, char state, int value1 );
    void print( Kind kind, unsigned int lid, char state, int value1, int value2 );

  private:
    unsigned int numStudents, numVendingMachines, numCouriers;
    char *buffer;
    unsigned int totalCol;
    //unsigned int searchPos( Kind kind );
    void inputNum( unsigned int &pos, int value );
    unsigned int searchPos( Kind kind, int lid );
    void printHeader( std::string s, int &pos, int &id );
    void flush();
    bool checkBuffer( unsigned int id );
    void printBuffer();
};

#endif
