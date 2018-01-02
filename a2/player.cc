#include <algorithm>
#include "player.h"
#include <iostream>
using namespace std;


unsigned int Player::numCurrentPlayers = 0;
unsigned int Player::numRemainCards = 0;
_Event Schmilblick {};

void Player::players( unsigned int num ) {
    Player::numCurrentPlayers = num;                            // Set number of total players
}

Player::Player( Printer &printer, unsigned int id ) {
    Player::left = NULL;
    Player::right = NULL;
    Player::raiseSchmilblick = false;
    Player::id = id;
    Player::printer = &printer;
}

void Player::start( Player &lp, Player &rp ) {
    if ( Player::left == NULL && Player::right == NULL ) {      // If my left&right are yet to
                                                                //  be set, store these 2 players
    	Player::left = &lp;
    	Player::right = &rp;
        resume();
    }

    if ( &lp == &rp ) {                                         // If both players are the same,
                                                                //  it means that now there are only
                                                                //  3 players remain, and the player who
                                                                //  called this funcion is deleting itself
                                                                //  from the game
        Player::left == &lp ? Player::left = Player::right : Player::right = Player::left;
        return;
    }

    if ( Player::left == &rp ) Player::left = &lp;              // The left player is deleting itself from game
    if ( Player::right == &lp ) Player::right = &rp;            // The right player is deleting itself from game
}

void Player::play( unsigned int deck ) {
    Player::numRemainCards = deck;
    resume();
}

void Player::drink() {
    _Enable {
        printer->prt( id, -2, Player::numCurrentPlayers );      // Send special value to printer
        _Resume Schmilblick() _At *right;                       // Raise Schmilblick to the player at right
        right->play( Player::numRemainCards );                  // Activate the player at right
    }
}

void Player::main() {
    suspend();

    for ( ;; ) {
        try {
            _Enable { 
		if ( Player::numCurrentPlayers <= 1) {                          // Last player wins the game
			printer->prt( id, 0, Player::numCurrentPlayers );       // Send msg to priner
			return;                                                 // Game ended, return to programe main
		} // if

		unsigned int draw = min( numRemainCards, prng( 1, 8 ) );        // Randomly pick up some cards
                printer->prt( id, draw, Player::numCurrentPlayers );            // Let printer print info of this play

                if ( numRemainCards % DEATH_DECK_DIVISOR != 0 ) {               // If the player is alive
                    unsigned int drinkCheck = prng( 0, 9 );                     // Schmilblick check
                    if ( drinkCheck == 0 && Player::numCurrentPlayers > 1 ) {   // If check passed and there are enough players
                        raiseSchmilblick = true;                                // Turn on flag variable as the starter of Schmilblick
                        printer->prt( id, -1, Player::numCurrentPlayers );      // Send special msg to the printer
                        Player::drink();                                        // Call drink(), prepare to raise exception to other players
                        raiseSchmilblick = false;                               // Schmilblick ended, turn off the flag
                    }
                }

		if ( draw == numRemainCards ) return;                           // The player picked the last card. Game ends

		if ( numRemainCards % DEATH_DECK_DIVISOR == 0 ) {               // If the deck is death deck
		    Player::numCurrentPlayers--;                                // Player delete itself from game
                    if ( numCurrentPlayers <= 2 ) {                             // Depending on number of remaining players, call start()
                                                                                //  in different way
                        left->start( *this, *this );
                        right->start( *this, *this );
                    } else {
			left->start( *this, *Player::right );
			right->start( *Player::left, *this );
                    }
		} // if

		Player::numRemainCards -= draw;                                 // Deduct the card drawn from the deck
		if ( Player::numRemainCards % 2 == 1 )                          // Next player is decided by the number of cards in deck
			left->play( Player::numRemainCards );
		else
			right->play( Player::numRemainCards );
            }
        } _CatchResume ( Schmilblick ) {                                        // Catch the Schmilblick exception
            if ( raiseSchmilblick ) {                                           // Is this player who raise the exception originally?
                printer->prt( id, -3, Player::numCurrentPlayers );              // Yes, so the circle finished, let the printer print
            } else {                                                            // No, call drink() and continue passing the exception to the right
                drink();
            }
        }
    } // for
}
