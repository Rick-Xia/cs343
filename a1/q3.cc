#include <iostream>
#include <fstream>
#include <string>
#include <sstream>
#include "q3.h"
using namespace std;
#include <cstdlib>

bool StringLiteral::isQuote() {				// Helper. Check if ch is a double quotation
        return ch == '"';
}

void StringLiteral::prefixCheck() {			// Check for valid encoding-prefix
        if ( isQuote() ) return;		// Empty prefix

        if ( ch == 'U' || ch == 'L' ) {
            suspend();
            if ( isQuote() ) return;
        } else if ( ch == 'u' ) {
            suspend();
            if ( isQuote() ) return;
            else if ( ch == '8' ) {
                suspend();
                if ( isQuote() ) return;
            }
        }
        _Throw Error();			        // Invalid encoding-prefix. Throw an Error
}

bool StringLiteral::isCharSimpleEscapeSequence() {	        // Check for valid simple-escape-sequence
        return ch == '\'' || ch == '"' || ch == '?' || ch == '\\' || ch == 'a' ||
            ch == 'b' || ch == 'f' || ch == 'n' || ch == 'r' || ch == 't' ||
            ch == 'v';
}

bool StringLiteral::isCharOct() {				// Check for a valid octal-escape-sequence
        return ch >= '0' && ch <= '7';
}

bool StringLiteral::isCharHex() {				// Check for valid hexadecimal-escape-sequence
        return ( ch >= '0' && ch <= '9' ) || ( ch >= 'a' && ch <= 'f' ) ||
                ( ch >= 'A' && ch <= 'F' );
}

void StringLiteral::escapeSequenceCheck() {		// Check for a valid escape-sequence.
        suspend();
        if ( isCharSimpleEscapeSequence() ) return;
        else if ( isCharOct() ) return;
        else if ( ch == 'x' ) {
            suspend();
            if ( isCharHex() ) return;
        }
        _Throw Error();			        // Invalid escape-sequence. Throw an Error
}

void StringLiteral::sCharCheck() {
        for ( ;; ) {
            suspend();
            if ( isQuote() ) throw Match();		// We found the closing quotation. A Match has been raised
            if ( ch == '\\' ) escapeSequenceCheck();	// Backslash. going to validate this escacpe sequence
        }
}

void StringLiteral::main() {                                       // coroutine main
        try {
        	suspend();				// ctor activates the coroutine
        	prefixCheck();			        // check for valid prefix( including the opening
        						//	double quotation )
        	sCharCheck();			        // check for valid s-char-seq( including the closing
        						//	double quotation )

        } _Catch ( Match ) {			        // A match has raised. Print all chars we read
        	cout << "'" << readHistory << "' yes";
        	_Throw;				        // Rethrow. Telling main that reading is completed
        } _Catch ( Error ) {			        // A error has raised. Print all chars we read
        	cout << "'" << readHistory << "' no";
        	_Throw;				        // Rethrow. Telling main that reading is completed
        } // try
}

StringLiteral::StringLiteral() {
	ch = '';
	readHistory = "";
	resume();
}

StringLiteral::~StringLiteral() { cout << endl; }

void StringLiteral::next( char c ) {
    StringLiteral::ch = c;
    StringLiteral::readHistory += c;                        // communication input
    resume();                       	                // activate
}

void printIfBlankLine( string &line ) {				// Special output for blank line
    cout << "Warning! Blank line.";
}

void printUnfinishedString( string &line ) {			// Unifished string is simply
								//  not recognized
    cout << "'" << line << "' no";
}

void printExtraneousChar( string &line, int numCharRead ) {	// Chars following to a valid
								//  or invalid C++ stringliteral should be
								//  left untouched
    if ( numCharRead < line.length() ) {
        cout << " -- extraneous characters ";
        cout << "'" << line.substr( numCharRead ) << "'";
    }
}

void printWholeLine( string &line ) {		                // Print the line which to be read
    cout << "'" << line << "' : ";
}

void uMain::main() {
    istream *infile = &cin;				        // default value

    switch ( argc ) {
        case 2: try {
        	infile = new ifstream( argv[1] );
        } catch( uFile::Failure ) {
        	cerr << "Error! Could not open input file \"" << argv[1] << "\"" << endl;
        	exit( EXIT_FAILURE );
        } // try
        case 1: break;						// all defaults
        default: cerr << "Usage: " << argv[0] << " [infile-file] " << endl;
      	   exit( EXIT_FAILURE );
    } // switch

    string line;
    char read;
    for ( ;; ) {
        getline( *infile, line );
      if ( infile->fail() ) break;				// EOF reached. Return
        istringstream is(line);
        is >> noskipws;					        // do not skip spaces
        StringLiteral sl;
        int numCharRead = 0;

        try {
            printWholeLine( line );
            for ( ;; ) {
                is >> read;
              if ( is.fail() ) break;
                numCharRead++;
                sl.next( read );
            } // for
        } catch ( uBaseCoroutine::UnhandledException ) {	                // Catch the rethrow from coroutine
        									//  print untouched remaining chars
        									//  ( if any )
            printExtraneousChar( line, numCharRead );
            continue;							        // continue to read next line
        } // try

        // Reach here if no Match or Error raised. Simply not recognize the input
        if ( line == "" ) printIfBlankLine( line );		// if input is empty
        else printUnfinishedString( line );			// if input is an unfinished string
    } // for

    if ( infile != &cin ) delete infile;			// Garbage collect
}
