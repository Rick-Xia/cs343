#include "q3buffer.h"
#include "q3producer.h"
#include "q3consumer.h"
#include "MPRNG.h"
#include <string>
#include <iostream>
#include <cassert>
using namespace std;

MPRNG RANDOM;
#define WRONG_ARG -1


/*              Start of the BoundedBuffer implementaion                */

template<typename T>
BoundedBuffer<T>::BoundedBuffer( const unsigned int size ) : size( size ) {             // ctor
    internalBuffer = new T[size];
} // BoundedBuffer<T>::BoundedBuffer

template<typename T> BoundedBuffer<T>::~BoundedBuffer() { delete internalBuffer; }      // dtor

template<typename T>
bool BoundedBuffer<T>::isBufferEmpty() {                // Return true if there are no data available
    return numOfStorage == 0;
} // BoundedBuffer<T>::isBufferEmpty

template<typename T>
bool BoundedBuffer<T>::isBufferFull() {                 // Return true if the buffer is full
    return numOfStorage == size;
} // BoundedBuffer<T>::isBufferFull

template<typename T>
T BoundedBuffer<T>::bufferPop() {                       // Pop out the first data and update the
                                                        //  consumerIndex and numOfStorage
    consumerIndex = ( consumerIndex + 1 ) % size;
    T ret = internalBuffer[ consumerIndex ];
    numOfStorage -= 1;
    return ret;
} // BoundedBuffer<T>::bufferPop

template<typename T>
void BoundedBuffer<T>::bufferPush( T elem ) {           // Push data into the buffer and update the
                                                        //  producerIndex and numOfStorage
    internalBuffer[ producerIndex ] = elem;
    producerIndex = ( producerIndex + 1 ) % size;
    numOfStorage += 1;
} // BoundedBuffer<T>::bufferPush

#ifdef BUSY
template<typename T>
void BoundedBuffer<T>::insert( T elem ) {
    curOwnerLock.acquire();	                        // Enter the critical section

    for( ;; ) {         // Busy waiting
        if ( !isBufferFull() ) break;	                // If there is a free spot, continue the insertion
	insertionLock.wait( curOwnerLock );             // No such a spot, wait inside
                                                        //  the insertionLock for a consumer to wake me up
    } // for

    assert( !isBufferFull() );		// Sanity check

    bufferPush( elem );                                 // Insert the data
    if( !removalLock.empty() ) removalLock.signal();	// Wake up a waiting consumer

    curOwnerLock.release();                             // Leave the critical section
} // BoundedBuffer<T>::insert

template<typename T>
T BoundedBuffer<T>::remove() {
    curOwnerLock.acquire();                             // Enter the critical section

    for( ;; ) {         // Busy waiting
	if ( !isBufferEmpty() ) break;                  // Buffer is not empty, continue the consumption
	removalLock.wait( curOwnerLock );               // Nothing to consume, wait inside
                                                        //  the removalLock for a producer to wake me up
    } // for

    assert( !isBufferEmpty() );         // Sanity check

    T retValue = bufferPop();                           // Get the front value

    if( !insertionLock.empty() ) insertionLock.signal();// Wake up a waiting producer

    curOwnerLock.release();                             // Leave the critical section
    return retValue;
} // BoundedBuffer<T>::remove
#endif // BUSY

#ifdef NOBUSY
template<typename T>
void BoundedBuffer<T>::bargeCheck() {
    if( bargeFlag ) {                                   // Check if the thread is barging
	bargeLock.wait( curOwnerLock );                 // It is barging, put it to sleep
	bargeFlag = !bargeLock.empty();                 // Keep the flag if there are threads waiting in bargeLock
    } // if
} // BoundedBuffer<T>::bargeCheck

template<typename T>
void BoundedBuffer<T>::insert( T elem ) {
    curOwnerLock.acquire();                             // Enter the critical section
    bargeCheck();                                       // Barging check

    if ( isBufferFull() ) {                             // If there is a free slot, continue the insertion
	if ( !bargeLock.empty() ) {                     // No free slot available, check if there are any
                                                        //  barging threads to wake up
	    bargeFlag = true;                           // There are. Raise the flag for avoiding
                                                        //  new barging threads
	    bargeLock.signal();                         // Wake up barging thread
	}
	insertionLock.wait( curOwnerLock );             // Wait inside the insertionLock for
                                                        //  a consumer to wake me up
	bargeFlag = !bargeLock.empty();                 // Keep the flag if there are threads waiting in bargeLock
    }

    assert( !isBufferFull() );          // Sanity check
    bufferPush( elem );                                 // Insert the data

    if ( !removalLock.empty() ) {                       // Check if there are any waiting consumers
	bargeFlag = true;                               // There are. Raise the flag for avoiding
                                                        //  new barging threads
	removalLock.signal();                           // Wake up waiting consumer
    } else if ( !bargeLock.empty() ) {                  // No waiting consumers, check if there are any
                                                        //  waiting bargers
	bargeFlag = true;                               // There are. Raise the flag for avoiding
                                                        //  new barging threads
	bargeLock.signal();                             // Wake up a barging thread
    }

    curOwnerLock.release();                             // Leave the critical section
} // BoundedBuffer<T>::insert

template<typename T>
T BoundedBuffer<T>::remove() {
    curOwnerLock.acquire();                             // Enter the critical section
    bargeCheck();                                       // Barging check

    if ( isBufferEmpty() ) {                            // If there are data available, continue the removal
	if ( !bargeLock.empty() ) {                     // No data available, check if there are any
                                                        //  barging threads to wake up
	    bargeFlag = true;                           // There are. Raise the flag for avoiding
                                                        //  new barging threads
	    bargeLock.signal();                         // Wake up barging thread
	}
	removalLock.wait( curOwnerLock );               // Wait inside the removalLock for
                                                        //  a producer to wake me up
	bargeFlag = !bargeLock.empty();                 // Keep the flag if there are threads waiting in bargeLock
    }

    assert( !isBufferEmpty() );         // Sanity check
    T retValue = bufferPop();                           // Retrieve the data

    if ( !insertionLock.empty() ) {                     // Check if there are any waiting producers
	bargeFlag = true;                               // There are. Raise the flag for avoiding
                                                        //  new barging threads
	insertionLock.signal();                         // Wake up a waiting producer
    } else if ( !bargeLock.empty() ) {                  // No waiting producers, check if there are any
                                                        //  waiting bargers
        bargeFlag = true;                               // There are. Raise the flag for avoiding
                                                        //  new barging threads
	bargeLock.signal();                             // Wake up barging thread
    }

    curOwnerLock.release();                             // Leave the critical section
    return retValue;
} // BoundedBuffer<T>::remove
#endif // NOBUSY

/*              End of the BoundedBuffer implementaion                */

/*              Start of the Consumer implementaion                */

Consumer::Consumer( BoundedBuffer<int> &buffer, const int Delay, const int Sentinel, int &sum ) : buffer(buffer), Delay(Delay), Sentinel(Sentinel), sum(sum) {}

void Consumer::main() {
    for ( ;; ) {
	int vacationWithSalary = RANDOM( 0, Delay - 1 );
	yield( vacationWithSalary );                            // Randomly yield [0, Delay-1] times
	int valueRetrieve = buffer.remove();                    // Retrieve a data

      if ( valueRetrieve == Sentinel ) break;                   // Stop if SENTINEL received
	sum += valueRetrieve;                                   // Sum up what it got
    } // for
} // Consumer::main

/*              End of the Consumer implementaion                */

/*              Start of the Producer implementaion                */

Producer::Producer( BoundedBuffer<int> &buffer, const int Produce, const int Delay ) : buffer(buffer), Produce(Produce), Delay(Delay) {}

void Producer::main() {
    for ( int i = 0 ; i < Produce; ++i ) {                      // Produce $Produce integers
	int vacationWithoutSalary = RANDOM( 0, Delay - 1 );
	yield( vacationWithoutSalary );                         // Randomly yield [0, Delay-1] times
	buffer.insert( i + 1 );                                 // Insert the data
    }
} // Producer::main

/*              End of the Producer implementaion                */

/*              Start of the main program implementaion                */

void usage( char* c ) {
    cerr << "Usage: " << c << " [ Cons (> 0) [ Prods (> 0) [ Produce (> 0) [ BufferSize (> 0) [ Delay (> 0) ] ] ] ] ]" << endl;
}

void uMain::main() {
    int Cons = 5, Prods = 3, Produce = 10;
    int BufferSize = 10;
    int Delays = 8;

    try {
	switch( argc ) {
	    case 6:
		Delays = stoi( argv[5] );
		if ( Delays < 0 ) throw WRONG_ARG;
		// FALL THROUGH
	    case 5:
		BufferSize = stoi( argv[4] );
		if ( BufferSize < 0 ) throw WRONG_ARG;
	        // FALL THROUGH
	    case 4:
		Produce = stoi( argv[3] );
		if ( Produce < 0 ) throw WRONG_ARG;
	        // FALL THROUGH
	    case 3:
	        Prods = stoi( argv[2] );
		if ( Prods < 0 ) throw WRONG_ARG;
		// FALL THROUGH
	    case 2:
		Cons = stoi( argv[1] );
		if ( Cons < 0 ) throw WRONG_ARG;
		// FALL THROUGH
	    case 1:
                if ( argc > 1 && argc < 6 ) Delays = Prods + Cons;
		break;

	    default:
		throw WRONG_ARG;
	} // switch
    } catch ( ... ) {
	usage( argv[0] );
        exit( EXIT_FAILURE );
    } // try

#ifdef __U_MULTI__
	uProcessor p[3] __attribute__ (( unused )); // create 3 kernel thread for a total of 4
#endif // __U_MULTI__

    BoundedBuffer<int> buffer( BufferSize );
    Producer* producers[ Prods ];
    Consumer* consumers[ Cons ];
    int subtotals[ Cons ];                                              // Partial result
    int total = 0;                                                      // Final result

    for ( int i = 0; i < Prods; ++i ) {                                 // Create Prods tasks
	producers[ i ] = new Producer( buffer, Produce, Delays );
    } // for

    for ( int i = 0; i < Cons; ++i ) {                                  // Create Cons tasks
	int& sum = subtotals[ i ];
	sum = 0;
	consumers[ i ] = new Consumer( buffer, Delays, SENTINEL, sum ); // Pass the ref of partial result
    } // for

    for( int j = 0; j < Prods; ++j ) {                                  // Wait for all Producers to
                                                                        //  finish all computation
	delete producers[ j ];
    } // for

    for ( int j = 0; j < Cons; ++j ) {                                  // Add sentinel values for all consumers
	buffer.insert( SENTINEL );
    } // for
    for( int j = 0; j < Cons; ++j ) {                                   // Wait for all consumers to
                                                                        //  finish their computation
        delete consumers[ j ];
	total += subtotals[ j ];
    }

    cout << Cons << " " << Prods << " " << Produce << " " << BufferSize << " " << Delays << endl;
    cout << "total: " << total << endl;
} // uMain::main
