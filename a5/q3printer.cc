#include "q3printer.h"
#include <iostream>
#include <string>
using namespace std;

Printer::Printer( unsigned int voters ) : voters(voters) {
    Printer::buffer = new char[ 8 * voters ];                           // Printer buffer, which consists of 8 char per voter
    Printer::flush();
    string delim = "", star = "*******";

    for ( unsigned int i = 0; i < voters; ++i ) {                       // Print the header info
        cout << "V" << i << '\t';
        delim += star + '\t';
    } // for
    cout << endl;
    cout << delim << endl;                                              // Print the separating stars
} // Printer::Printer

Printer::~Printer() {
    for ( unsigned int i = 0; i < Printer::voters; ++i ) {              // Check if there are remaining info yet to be printed before we delete them
        if ( Printer::checkBuffer( i ) ) {
            Printer::printBuffer();
            break;
        } // if
    } // for
    delete Printer::buffer;
    cout << "*****************" << endl;                                // Print the ending info
    cout << "All tours started" << endl;
} // Printer::~Printer

void Printer::flush() {                                                 // Helper that flushes the buffer
    int len = 8 * Printer::voters;
    for ( int i = 0; i < len; ++i ) {
        if ( i % 8 == 0) {
            Printer::buffer[ i ] = '\t';                               // Set the first char of the buffer for each voter to be \t
        } else {
            Printer::buffer[ i ] = NULL;                               // Set all other chars to be NULL
        } // if
    } // for
} // Printer::flush

void Printer::printBuffer() {                                          // Helper that print the buffer on screen
    for ( unsigned int i = 0; i < 8 * Printer::voters; ++i ) {
        if ( Printer::buffer[ i ] == NULL ) continue;
        cout << Printer::buffer[ i ];
    } // for
    cout << endl;
    Printer::flush();                                                  // Printing done, flush the buffer
} // Printer::printBuffer

bool Printer::checkBuffer( unsigned int id ) {                         // Helper that check if there are info stored in the buffer of the id-th voter
    return Printer::buffer[ 8 * id ] != '\t';
} // Printer::checkBuffer

void Printer::print( unsigned int id, Voter::States state ) {          // Store info into the buffer
    if ( Printer::checkBuffer( id ) ) Printer::printBuffer();

    unsigned int pos = 8 * id;
    Printer::buffer[ pos++ ] = state;
    Printer::buffer[ pos++ ] = '\t';
} // Printer::print

void Printer::print( unsigned int id, Voter::States state, TallyVotes::Tour tour ) {          // Store info into the buffer
    if ( Printer::checkBuffer( id ) ) Printer::printBuffer();

    unsigned int pos = 8 * id;
    Printer::buffer[ pos++ ] = state;
    Printer::buffer[ pos++ ] = ' ';
    Printer::buffer[ pos++ ] = tour;
    Printer::buffer[ pos++ ] = '\t';
} // Printer::print

void Printer::print( unsigned int id, Voter::States state, TallyVotes::Ballot ballot ) {          // Store info into the buffer
    if ( Printer::checkBuffer( id ) ) Printer::printBuffer();

    unsigned int pos = 8 * id;
    Printer::buffer[ pos++ ] = state;
    Printer::buffer[ pos++ ] = ' ';
    Printer::buffer[ pos++ ] = ballot.picture + 48;
    Printer::buffer[ pos++ ] = ',';
    Printer::buffer[ pos++ ] = ballot.statue + 48;
    Printer::buffer[ pos++ ] = ',';
    Printer::buffer[ pos++ ] = ballot.giftshop + 48;
    Printer::buffer[ pos++ ] = '\t';
} // Printer::print

void Printer::print( unsigned int id, Voter::States state, unsigned int numBlocked ) {          // Store info into the buffer
    if ( Printer::checkBuffer( id ) ) Printer::printBuffer();

    unsigned int pos = 8 * id;
    Printer::buffer[ pos++ ] = state;
    Printer::buffer[ pos++ ] = ' ';
    string tmp = to_string( numBlocked );
    for ( char c : tmp ) {
        Printer::buffer[ pos++ ] = c;
    }
    Printer::buffer[ pos++ ] = '\t';
} // Printer::print
