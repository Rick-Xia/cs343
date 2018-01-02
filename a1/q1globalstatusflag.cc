#include <iostream>
#include <cstdlib>                      // access: rand, srand
using namespace std;
#include <unistd.h>                     // access: getpid
#include <limits.h>                      // access: INT_MAX

#define NO_ERROR INT_MAX

int times = 1000;                       // default value
int ERRNO = NO_ERROR;                  // global status flag

void rtn1( int i ) {
    for ( int j = 0; j < times; j += 1 ) {
        if ( rand() % 10000 == 42 ) {
            ERRNO = j;
            break;
        }
    }
}
void rtn2( int i ) {
    for ( int j = times; j >= 0; j -= 1 ) {
        if ( rand() % 10000 == 42 ) {
            ERRNO = j;
            break;
        }
    }
}
void g( int i ) {
    for ( int j = 0; j < times; j += 1 ) {
        if ( rand() % 2 == 0 ) rtn1( i );
        else rtn2( i );

        if ( ERRNO != NO_ERROR ) return;
    }
    if ( i % 2 ) rtn2( i );
   if ( ERRNO != NO_ERROR ) return;
    rtn1( i );
}
void f( int i ) {
    for ( int j = 0; j < times; j += 1 ) {
        g( i );
       if ( ERRNO != NO_ERROR ) return;
    }
    if ( i % 2 ) g( i );
   if ( ERRNO != NO_ERROR ) return;
    g( i );
}
int main( int argc, char *argv[] ) {
    int seed = getpid();                // default value
    try {                                // process command-line arguments
        switch ( argc ) {
          case 3: times = stoi( argv[2] ); if ( times <= 0 ) throw 1;
          case 2: seed = stoi( argv[1] ); if ( seed <= 0 ) throw 1;
          case 1: break;                // use defaults
          default: throw 1;
        } // switch
    } catch( ... ) {
        cout << "Usage: " << argv[0] << " [ seed (> 0) [ times (> 0) ] ]" << endl;
        exit( 1 );
    } // try
    srand( seed );			// set random-number seed
    f( 3 );                    // begin program
    if ( ERRNO == NO_ERROR )
        cout << "seed:" << seed << " times:" << times << " complete" << endl;
    else
        cout << "seed:" << seed << " times:" << times << " rc:" << ERRNO << endl;
}
