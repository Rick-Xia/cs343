#include "printer.h"
#include <iostream>
using namespace std;


Printer::Printer( const unsigned int NoOfPlayers, const unsigned int NoOfCards ) {
	Printer::NoOfPlayers = NoOfPlayers;
	Printer::NoOfCards = NoOfCards;
	Printer::printerBuffer = new char[ 8 * NoOfPlayers ];                           // Printer buffer, it consist of 8 char per player
	flush();

	cout << "Players: " << NoOfPlayers << "    Cards: " << NoOfCards << endl;       // Print basic info of this round of game
	for ( unsigned int i = 0; i < NoOfPlayers; ++i ) {
		cout << "P" << i << '\t';
	}
	cout << endl;
}

void Printer::flush() {                                                                 // Flush the buffer
	int len = 8 * Printer::NoOfPlayers;
	for ( int i = 0; i < len; ++i ) {
		if ( i % 8 == 0) {                                                      // If it is the first char of a player
			Printer::printerBuffer[i] = '\t';                               // Set the char to be \t
		} else {                                                                // If not
			Printer::printerBuffer[i] = NULL;                               // Set it to be NULL
		}
	}
}

void Printer::convertAndWriteInBuffer( unsigned int id, int &index, int deck ) {        // Helper, write the size of deck into the buffer
	if ( deck >= 100 ) {
		printerBuffer[ 8 * id + ( index++ ) ] = deck / 100 + 48;
		deck = deck % 100;
            if ( deck < 10 )
                printerBuffer[ 8 * id + ( index++ ) ] = '0';
	}

	if ( deck >= 10 ) {
		printerBuffer[ 8 * id + ( index++ ) ] = deck / 10 + 48;
		deck = deck % 10;
	}

	printerBuffer[ 8 * id + ( index++ ) ] = deck + 48;
}

void Printer::printBuffer() {                                                           // Print the info stored in buffer on screen
	for ( unsigned int i = 0; i < 8 * NoOfPlayers; ++i ) {
		if ( printerBuffer[ i ] == NULL ) continue;
		cout << printerBuffer[ i ];
	} // for
	cout << endl;
	flush();
}

Printer::~Printer() {
	delete Printer::printerBuffer;
}

void Printer::prt( unsigned int id, int took, int RemainingPlayers ) {
	if ( took == -1 ) {		                                                // Special msg 1. Check if the buffer is empty
                                                                                        //  if not, print them out and flush the buffer
	    for ( unsigned int i = 0; i < NoOfPlayers; ++i ) {
                if ( printerBuffer[ 8 * i ] == '\t' ) continue;
                printBuffer();
                break;
            }
            return;
        } else if ( took == -2 ) {                                                      // Special msg 2. Sign the player up to the Schmilblick
                                                                                        //  so later a 'D' will be printed under its name
            printerBuffer[ 8 * id ] = 'D';
            printerBuffer[ 8 * id + 1 ] = '\t';
            return;
        } else if ( took == -3 ) {                                                      // Special msg 3. The circle of Schmilblick finished,
                                                                                        //  print everything out
            printBuffer();
            return;
        } // if

	if ( printerBuffer[ 8 * id ] != '\t' ) Printer::printBuffer();                  // If the buffer is pre-written something. Print them out

	int index = 0;                                                                  // Index of position in the buffer
	int remainDeck = Printer::NoOfCards - took;

	if ( RemainingPlayers == 1 ) {			                                // last player, who wins the game!
		printerBuffer[ 8 * id + ( index++ ) ] = '#';
		convertAndWriteInBuffer( id, index, remainDeck );
		printerBuffer[ 8 * id + ( index++ ) ] = '#';

		if ( Printer::NoOfCards % 7 == 0 )                                      // Death deck. The player also died
			printerBuffer[ 8 * id + ( index++ ) ] = 'X';

		printerBuffer[ 8 * id + ( index++ ) ] = '\t';
		printBuffer();                                                          // As the last play, print the buffer
		return;                                                                 // Game ended, return to program main
	}

	printerBuffer[ 8 * id + ( index++ ) ] = took + 48;                              // Write the number of card drawn
	printerBuffer[ 8 * id + ( index++ ) ] = ':';                                    // Write the ':'
	convertAndWriteInBuffer( id, index, remainDeck );                               // Write the remaining number of cards in deck

	if ( remainDeck == 0 ) {                                                        // If remaining deck is empty, the player wins the game
		printerBuffer[ 8 * id + ( index++ ) ] = '#';
		if ( NoOfCards % 7 == 0 ) {
			printerBuffer[ 8 * id + ( index++ ) ] = 'X';
		}
		printerBuffer[ 8 * id + ( index++ ) ] = '\t';
		printBuffer();
		return;
	}

	if ( remainDeck % 2 == 1 )                                                      // Print the direction of next play
		printerBuffer[ 8 * id + ( index++ ) ] = '<';
	else
		printerBuffer[ 8 * id + ( index++ ) ] = '>';

	if ( NoOfCards % 7 == 0 )
		printerBuffer[ 8 * id + ( index++ ) ] = 'X';

	printerBuffer[ 8 * id + ( index++ ) ] = '\t';                                   // Writing on the buffer finishes. Put \t at the end

	Printer::NoOfCards = remainDeck;
}
