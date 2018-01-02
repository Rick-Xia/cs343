#include <iostream>
#include <fstream>
#include <string>
#include <sstream>
#include "q1binsertsort.h"
#include <cstdlib>
using namespace std;

#ifndef TYPE
#define TYPE int
#endif

struct SampleStruct {
    int value;
    bool isRaininigToday;
    SampleStruct( int v, bool b ) { value = v; isRaininigToday = b; }
    ~SampleStruct();
    bool operator ==( SampleStruct b ) {
        return this->value == b.value;
    }
    bool operator <( SampleStruct b ) {
        return this->value < b.value;
    }
    void operator <<( int i ) {
        this->value = i;
    }
    void operator >>( int i ) {
        i = this->value;
    }
};

template<typename T>
void Binsertsort<T>::main() {
	Binsertsort<T> less( Binsertsort::Sentinel );                   // Left child
	Binsertsort<T> greater( Binsertsort::Sentinel );                // Right child

	T pivot = Binsertsort::value;                                   // Store the pivot value in local val
	suspend();                                                      // Basic setting done. suspend

	for ( ;; ) {                                                    // Keep receive input from program main
	  if( Binsertsort::value == Binsertsort::Sentinel ) break;      // If a SENTINEL is recived, stop reading
	    if( Binsertsort::value < pivot ) less.sort( value );        // If v < pivot, resume left child
	    if( Binsertsort::value >= pivot ) greater.sort( value );    // If v >= pivot, resume right child
	    suspend();                                                  // Waiting for the next input
	} // for
	suspend();                                                      // Receiving input stopped, wait for retrieve()

	less.sort( Binsertsort::Sentinel );                             // Turn off the input reading of this node's left
                                                                        //  child, prepare to receive the sorted value from
                                                                        //  the left branch

	for ( ;; ) {                                                    // Recursion through the left branch
	    Binsertsort::value = less.retrieve();                       // Store the value returned from children nodes
	  if ( Binsertsort::value == Binsertsort::Sentinel ) break;     // If the child terminated, break this recursive loop
                                                                        //  and prepare to return my own value
	    suspend();
	}

	Binsertsort::value = pivot;                                     // Store my own value into return value
	suspend();
	greater.sort( Binsertsort::Sentinel );                          // Turn off the input reading of this node's right
                                                                        //  child, prepare to retrieve their sorted value

	for ( ;; ) {
	    Binsertsort::value = greater.retrieve();                    // Recursion through the right branch. Same logic as above
	  if ( Binsertsort::value == Binsertsort::Sentinel ) break;
	    suspend();
	}
}


void uMain::main() {
    istream *infile = &cin;				        // default value
    ostream *outfile = &cout;				        // default value

    switch ( argc ) {
    	  case 3: try {
    	  	outfile = new ofstream( argv[2] );              // try open the output file given from command line
    	  } catch ( uFile::Failure ) {
    	  	cerr << "Error! Could not open output file \"" << argv[2] << "\"" << endl;
    	  	exit( EXIT_FAILURE );
    	  } // try
    	  // FALL THROUGH
        case 2: try {
        	infile = new ifstream( argv[1] );               // try open the input file given from command line
        	break;
        } catch( uFile::Failure ) {
        	cerr << "Error! Could not open input file \"" << argv[1] << "\"" << endl;
        	exit( EXIT_FAILURE );
        } // try
        default: cerr << "Usage: " << argv[0] << " unsorted-file [ sorted-file ] " << endl;
      	   exit( EXIT_FAILURE );
    } // switch

    int numOfInput = 0;                                 // Indicator of the number of each set, should always be the first number
    int read = 0;

    for ( ;; ) {
        *infile >> numOfInput;                          // Read the indicator
      if ( infile->fail() ) break;                      // EOF reached, stop reading input
        Binsertsort<TYPE> top( SENTINEL );              // Initialize the top node
        for ( int i = 0; i < numOfInput; ++i ) {        // Start to read numbers of the set
            *infile >> read;
            *outfile << read << " ";                    // Print read value to specified output stream
            top.sort(read);                             // Give the value to the top node, letting it to sort
        } // for
        *outfile << endl;
        top.sort( SENTINEL );                           // Turn off the input receiving of the top node
        for ( int i = 0; i < numOfInput; ++i ) {        // Start to retrieve sorted value from the top node
        	int ret = top.retrieve();
        	*outfile << ret << " ";                 // Print the value we retrieved
        }
        *outfile << endl << endl;                       // Formatting given by the example executable
    } // for


    if ( infile != &cin ) delete infile;	        // close file, do not delete cin!
    if ( outfile != &cout ) delete outfile;		// close file, do not delete cout!
}
