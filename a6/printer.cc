#include "printer.h"
#include <iostream>
#include <string>
using namespace std;

void Printer::printHeader( string s, int &pos, int &id ) {
    for ( char c : s ) Printer::buffer[ pos++ ] = c;
    Printer::buffer[ pos++ ] = '\t';
    id = id + 1;
    pos = id * 8;
}

Printer::Printer( unsigned int numStudents, unsigned int numVendingMachines, unsigned int numCouriers ) : numStudents(numStudents), numVendingMachines(numVendingMachines), numCouriers(numCouriers) {
    Printer::totalCol = 6 + numStudents + numVendingMachines + numCouriers;
    Printer::buffer = new char[ 8 * Printer::totalCol ];                           // Printer buffer, which consists of 8 char per object
    Printer::flush();

    int pos = 0, id = 0;
    printHeader( "Parent", pos, id );
    printHeader( "Gropoff", pos, id );
    printHeader( "WATOff", pos, id );
    printHeader( "Names", pos, id );
    printHeader( "Truck", pos, id );
    printHeader( "Plant", pos, id );
    for ( unsigned int i = 0; i < numStudents; ++i ) {                       // Print the header info
        printHeader( "Stud" + to_string(i), pos, id );
    } // for
    for ( unsigned int i = 0; i < numVendingMachines; ++i ) {                       // Print the header info
        printHeader( "Mach" + to_string(i), pos, id );
    } // for
    for ( unsigned int i = 0; i < numCouriers; ++i ) {                       // Print the header info
        printHeader( "Cour" + to_string(i), pos, id );
    } // for
    Printer::printBuffer();

    string star = "*******";
    pos = 0; id = 0;
    for ( unsigned int i = 0; i < totalCol; ++i ) {
        printHeader( star, pos, id);                                              // Print the separating stars
    }
    Printer::printBuffer();
}

Printer::~Printer() {
    for ( unsigned int i = 0; i < Printer::totalCol; ++i ) {              // Check if there are remaining info yet to be printed before we delete them
        if ( Printer::checkBuffer( i ) ) {
            Printer::printBuffer();
            break;
        } // if
    } // for
    delete [] Printer::buffer;
    cout << "***********************" << endl;                                // Print the ending info
} // Printer::~Printer

/*
unsigned int Printer::searchPos( Kind kind ) {
    return 8 * kind;
}
*/

unsigned int Printer::searchPos( Kind kind, int lid = -1 ) {
    if ( lid == -1 ) return 8 * kind;

    switch ( kind ) {
        case 6: return ( 6 + lid ) * 8;
        case 7: return ( 6 + Printer::numStudents + lid ) * 8;
        case 8: return ( 6 + Printer::numStudents + Printer::numVendingMachines + lid ) * 8;
        default: return 0;
    }
}

void Printer::flush() {                                                 // Helper that flushes the buffer
    int len = 8 * Printer::totalCol;
    for ( int i = 0; i < len; ++i ) {
        if ( i % 8 == 0) {
            Printer::buffer[ i ] = '\t';                               // Set the first char of the buffer for each voter to be \t
        } else {
            Printer::buffer[ i ] = NULL;                               // Set all other chars to be NULL
        } // if
    } // for
} // Printer::flush

void Printer::printBuffer() {                                          // Helper that print the buffer on screen
    for ( unsigned int i = 0; i < 8 * Printer::totalCol; ++i ) {
      if ( Printer::buffer[ i ] == NULL ) continue;
        cout << Printer::buffer[ i ];
    } // for
    cout << endl;
    Printer::flush();                                                  // Printing done, flush the buffer
} // Printer::printBuffer

bool Printer::checkBuffer( unsigned int pos ) {                         // Helper that check if there are info stored in the buffer of the id-th voter
    return Printer::buffer[ pos ] != '\t';
    //return true;
} // Printer::checkBuffer

void Printer::inputNum( unsigned int &pos, int value ) {
    string tmp = to_string( value );
    for ( char c : tmp ) {
        Printer::buffer[ pos++ ] = c;
    }
}

void Printer::print( Kind kind, char state ) {
    unsigned int pos = Printer::searchPos( kind );
    if ( Printer::checkBuffer( pos ) ) Printer::printBuffer();

    Printer::buffer[ pos++ ] = state;
    Printer::buffer[ pos++ ] = '\t';
}

void Printer::print( Kind kind, char state, int value1 ) {
    unsigned int pos = Printer::searchPos( kind );
    if ( Printer::checkBuffer( pos ) ) Printer::printBuffer();

    Printer::buffer[ pos++ ] = state;
    Printer::inputNum( pos, value1 );
    Printer::buffer[ pos++ ] = '\t';
}

void Printer::print( Kind kind, char state, int value1, int value2 ) {
    unsigned int pos = Printer::searchPos( kind );
    if ( Printer::checkBuffer( pos ) ) Printer::printBuffer();

    Printer::buffer[ pos++ ] = state;
    Printer::inputNum( pos, value1 );
    Printer::buffer[ pos++ ] = ',';
    Printer::inputNum( pos, value2 );
    Printer::buffer[ pos++ ] = '\t';
}

void Printer::print( Kind kind, unsigned int lid, char state ) {
    unsigned int pos = Printer::searchPos( kind, lid );
    if ( Printer::checkBuffer( pos ) ) Printer::printBuffer();

    Printer::buffer[ pos++ ] = state;
    Printer::buffer[ pos++ ] = '\t';
}

void Printer::print( Kind kind, unsigned int lid, char state, int value1 ) {
    unsigned int pos = Printer::searchPos( kind, lid );
    if ( Printer::checkBuffer( pos ) ) Printer::printBuffer();

    Printer::buffer[ pos++ ] = state;
    Printer::inputNum( pos, value1 );
    Printer::buffer[ pos++ ] = '\t';
}

void Printer::print( Kind kind, unsigned int lid, char state, int value1, int value2 ) {
    unsigned int pos = Printer::searchPos( kind, lid );
    if ( Printer::checkBuffer( pos ) ) Printer::printBuffer();

    Printer::buffer[ pos++ ] = state;
    Printer::inputNum( pos, value1 );
    Printer::buffer[ pos++ ] = ',';
    Printer::inputNum( pos, value2 );
    Printer::buffer[ pos++ ] = '\t';
}
