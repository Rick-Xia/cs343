#ifndef __Q3PRINTER_H__
#define __Q3PRINTER_H__

#include "q3tallyVotes.h"
#include "q3voter.h"

_Monitor Printer {             // chose one of the two kinds of type constructor
    unsigned int voters;
    char *buffer;
    void flush();
    bool checkBuffer( unsigned int id );
    void printBuffer();
  public:
    Printer( unsigned int voters );
    ~Printer();
    void print( unsigned int id, Voter::States state );
    void print( unsigned int id, Voter::States state, TallyVotes::Tour tour );
    void print( unsigned int id, Voter::States state, TallyVotes::Ballot ballot );
    void print( unsigned int id, Voter::States state, unsigned int numBlocked );
};

#endif
