#ifndef __PLAYER_H__
#define __PLAYER_H__
#include "printer.h"
#include "PRNG.h"

extern PRNG prng;

_Coroutine Player {
    // YOU MAY ADD PRIVATE MEMBERS, INCLUDING STATICS
    unsigned int id;
    Printer *printer;
    Player *left;
    Player *right;
    bool raiseSchmilblick;
    static unsigned int numCurrentPlayers;
    static unsigned int numRemainCards;
    void main();
  public:
    enum { DEATH_DECK_DIVISOR = 7 };
    static void players( unsigned int num );
    Player( Printer &printer, unsigned int id );
    void start( Player &lp, Player &rp );
    void play( unsigned int deck );
    void drink();
};

#endif
