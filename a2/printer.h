#ifndef __PRINTER_H__
#define __PRINTER_H__

class Printer {
    // YOU MAY ADD PRIVATE MEMBERS
    unsigned int NoOfPlayers;
    unsigned int NoOfCards;
    char* printerBuffer;
    void flush();
    void printBuffer();
    void convertAndWriteInBuffer( unsigned int id, int &index, int deck );
  public:
    Printer( const unsigned int NoOfPlayers, const unsigned int NoOfCards );
    ~Printer();
    void prt( unsigned int id, int took, int RemainingPlayers );
};

#endif
