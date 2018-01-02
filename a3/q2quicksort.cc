#include <iostream>
#include <fstream>
#include <stdlib.h>
#include <unistd.h>
#include <cstring>
#include "q2quicksort.h"
#include <utility>
#include <climits>
using namespace std;

#define WRONG_ARG 1

#ifndef TYPE
#define TYPE int
#endif

void printArray( TYPE* array, int size, ostream& out ) {        // Helper that help print the array
    int printoutCount = 0;
    for ( int i = 0; i < size; ++i ) {
        out << array[i];
        printoutCount++;
        if ( printoutCount >= 22 && i != size - 1 ) {           // Start a new line after 22chars printed
            out << endl << "  ";
      	    printoutCount = 0;
    	} else if ( i < size - 1 ) out << " ";
    }
    out << endl;
}

template<typename T>
unsigned int Quicksort<T>::partition( unsigned int plow, unsigned int phigh ) { // Helper that divide an array into two using a pivot
    unsigned int pivotPos = plow + ( phigh - plow ) / 2;                        // Default choice of the pivot
    T pivot = array[ pivotPos ];

    swap( array[ pivotPos ], array[ plow ] );                                   // Put the pivot at the very beginning of the array
    unsigned int i = plow + 1;
    unsigned int j = phigh;
    while ( i <= j ) {
        while( i <= j && array[i] <= pivot ) i++;
        while( i <= j && array[j] > pivot ) j--;

        if ( i < j ) swap( array[i], array[j] );
    }
    swap( array[ i - 1 ], array[ plow ]);                                       // Move the pivot to the boundary of two parts
    return i - 1;                                                               // Return the position of the pivot in the sorted array
}

template<typename T>
void Quicksort<T>:: qSortMultiDepth( unsigned int mlow, unsigned int mhigh ) {  // Sorting algorithm for multi depth
    verify();
    if ( mlow >= mhigh || mhigh == UINT_MAX ) return;                           // Stop if no value to sort

    unsigned int sortedPosOfPivot = partition( mlow, mhigh );                   // Partition the array using a pivot

    if ( --depth == 0 ) {                                                       // Choose different sorting style based on depth
        Quicksort<T> sortFront( array, mlow, sortedPosOfPivot - 1, depth );
        qSortZeroDepth( sortedPosOfPivot + 1, mhigh );
    } else {
        Quicksort<T> sortFront( array, mlow, sortedPosOfPivot - 1, depth );
        qSortMultiDepth( sortedPosOfPivot + 1, mhigh );
    }
}

template<typename T>
void Quicksort<T>::qSortZeroDepth( unsigned int zlow, unsigned int zhigh ) {    // Sorting algorithm for zero depth
    verify();
    if ( zlow >= zhigh || zhigh == UINT_MAX ) return;                           // Stop if no values to sort

    unsigned int sortedPosOfPivot = partition( zlow, zhigh );                   // Partition the array using a pivot
    
    qSortZeroDepth( zlow, sortedPosOfPivot - 1 );                               // Sort two partitions recursively
    qSortZeroDepth( sortedPosOfPivot + 1, zhigh );
}

template<typename T>
void Quicksort<T>::main() {
    if ( high == UINT_MAX ) return;                                             // If this unsigned int equals to MAX, it indicates that an overflow happened
    return ( depth == 0 ) ? qSortZeroDepth( low, high ) :
                            qSortMultiDepth( low, high );                       // Continue with different sorting style based on depth
}

template<typename T>
Quicksort<T>::Quicksort( T values[], unsigned int low, unsigned int high, unsigned int depth ) :        // Ctor
    low(low), high(high), depth(depth) {
    Quicksort::array = values;
}

void usage( char* c ) {
    cerr << "Usage: " << c << " ( -s unsorted-file [ sorted-file ] | -t size (>= 0) [ depth (>= 0) ] )" << endl;
}

unsigned int uDefaultStackSize() {
    return 512 * 1000;        // set task stack-size to 512K
}

void uMain::main() {
    istream *infile = &cin;				        // default value
    ostream *outfile = &cout;				        // default value
    int size;
    int depth = 0;
    int mode = -1;		                                // Flag variable: 0 for sort mode, 1 for time mode
    
    try {
        if ( argc < 2 ) throw WRONG_ARG;
        if ( strcmp( argv[1], "-s" ) == 0 ) mode = 0;
        else if ( strcmp( argv[1], "-t" ) == 0 ) mode = 1;
        else throw WRONG_ARG;

        switch ( argc ) {
            case 4:
        	if ( !mode ) {
                    outfile = new ofstream( argv[3] );
        	} else if ( mode ) {
        	    depth = stoi( argv[3] );
        	    if ( depth < 0 ) throw WRONG_ARG;
        	} else throw WRONG_ARG;
        	// FALL THROUGH
            case 3:
        	if ( !mode ) {
                    infile = new ifstream( argv[2] );
        	} else if ( mode ) {
        	    size = stoi( argv[2] );
        	    if ( size < 0 ) throw WRONG_ARG;
        	} else throw WRONG_ARG;
        	break;

        	default: throw WRONG_ARG;
        }
    } catch ( ... ) {
        usage( argv[0] );
        exit( EXIT_FAILURE );
    }

    if ( !mode ) {
    	unsigned int numOfInput;                                                        // Number of values each input line
    	for ( ;; ) {
    	    *infile >> numOfInput;
    	  if ( infile->fail() ) break;
    	    TYPE* arrayToSort = new TYPE[ numOfInput ];                                 // Use a fixed size array to store values
    	    for ( unsigned int i = 0; i < numOfInput; ++i ) {
    	    	*infile >> arrayToSort[ i ];
    	    }
            printArray( arrayToSort, numOfInput, *outfile );                            // Print the unsorted array

    	    // START ALGORITHM
            if ( numOfInput != 0 ) {
    	        Quicksort<TYPE> sortArray( arrayToSort, 0, numOfInput - 1, depth );     // Initilize a sorting task
            }

            printArray( arrayToSort, numOfInput, *outfile );                            // Print the result

    	    delete arrayToSort;                                                         // Garbage collection
    	    *outfile << endl;
    	}
    } else {
    	uProcessor p[ (1 << depth) - 1 ] __attribute__(( unused ));                     // 2^depth-1 kernel threads

        int* arrayToSort = new int[ size ];                                             // Use a fixed size array to store values
        for ( int i = size; i > 0; --i ) {
            arrayToSort[ size - i ] = i;
        }
        
        if ( size != 0 ) {
            Quicksort<int> sortArray( arrayToSort, 0, size - 1, depth );                // Initilize a sorting task
        }
printArray( arrayToSort, size, *outfile );
        delete arrayToSort;                                                             // Garbage collection
    }

    if ( infile != &cin ) delete infile;	                                        // close file, do not delete cin!
    if ( outfile != &cout ) delete outfile;		                                // close file, do not delete cout!
}
