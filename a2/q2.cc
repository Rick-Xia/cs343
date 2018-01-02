#include <iostream>
#include <stdlib.h>
#include <unistd.h>
#include "PRNG.h"
#include "printer.h"
#include "player.h"
using namespace std;

PRNG prng;

void usage( char* c ) {
	cerr << "Usage: " << c << " [ games (>= 0) | \"x\" [ players (>= 2) | \"x\" [ cards (> 0) | \"x\" [ random-seed (> 0) | \"x\" ] ] ] ]" << endl;
}

void uMain::main() {
	int numOfGames = 5;
	int seed = getpid();
	prng.seed( seed );
	unsigned int numOfCards;
	unsigned int numOfPlayers;
	bool numOfPlayersGiven = false;                 // flag variable
	bool numOfCardsGiven = false;                   // flag variable
	try {
	switch ( argc ) {
		case 5: seed = stoi( argv[4] );
                        prng.seed( seed );
			if ( seed <= 0 ) {
				usage( argv[0] );
				exit( EXIT_FAILURE );
			}
		// FALL THROUGH
		case 4: numOfCards = stoi( argv[3] );
			numOfCardsGiven = true;
			if ( numOfCards <= 0 ) {
                                usage( argv[0] );
				exit( EXIT_FAILURE );
			}
		// FALL THROUGH
		case 3: numOfPlayers = stoi( argv[2] );
			numOfPlayersGiven = true;
			if ( numOfPlayers < 2 ) {
                                usage( argv[0] );
				exit( EXIT_FAILURE );
			}
		// FALL THROUGH
		case 2: numOfGames = stoi( argv[1] );
			if ( numOfGames < 0 ) {
                                usage( argv[0] );
				exit( EXIT_FAILURE );
			}
		// FALL THROUGH
		case 1: break;				// all default
		default: usage( argv[0] );
                         exit( EXIT_FAILURE );
	} // switch
} catch ( ... ) {
	usage( argv[0] );
      exit( EXIT_FAILURE );
}

	for ( int i = 0; i < numOfGames; ++i ) {
		if ( !numOfPlayersGiven ) numOfPlayers = prng( 2, 10 );                 // If num of players not given, randomly generate one
		if ( !numOfCardsGiven ) numOfCards = prng( 10, 200 );                   // If num of cards no given, randomly generate one

		Printer printer ( numOfPlayers, numOfCards );                           // Initilize the printer

		int startPlayer = prng( numOfPlayers - 1 );                             // Randomly pick a player as the first player

		Player::players( numOfPlayers );
		Player *players[ numOfPlayers ];
		for ( unsigned int j = 0; j < numOfPlayers; ++j ) {
			players[ j ] = new Player( printer, j );                        // Initilization of players
		}

                players[ 0 ]->start( *players[ numOfPlayers - 1 ], *players[ 1 ] );     // Register their left and right players
		for ( unsigned int j = 1; j < numOfPlayers; ++j ) {
			players[ j ]->start( *players[ ( j - 1 ) % numOfPlayers ],
					     *players[ ( j + 1 ) % numOfPlayers ] );
		}
		players[ startPlayer ]->play( numOfCards );                             // Let the first player start the game

		if ( i != numOfGames - 1 ) {
			cout << endl << endl;
		}

		for ( unsigned int j = 0; j < numOfPlayers; ++j ) {                     // A game ended, delete all players
			delete players[ j ];
		}
	}
}
