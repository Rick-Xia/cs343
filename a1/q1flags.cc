#include <iostream>
#include <cstdlib>                      // access: rand, srand
using namespace std;
#include <unistd.h>                     // access: getpid

int times = 1000;                       // default value

int rtn1( int i ) {
    int retCodeRtn1;
    bool loopFlagRtn1 = true;
    for ( int j = 0; j < times && loopFlagRtn1; j += 1 ) {
        if ( rand() % 10000 == 42 ) {
            retCodeRtn1 = j;
            loopFlagRtn1 = false;
        }
    }
    return loopFlagRtn1 ? -1 : retCodeRtn1;
}
int rtn2( int i ) {
    int retCodeRtn2;
    bool loopFlagRtn2 = true;
    for ( int j = times; j >= 0 && loopFlagRtn2; j -= 1 ) {
        if ( rand() % 10000 == 42 ) {
            retCodeRtn2 = j;
            loopFlagRtn2 = false;
        }
    }
    return loopFlagRtn2 ? -1 : retCodeRtn2;
}
int g( int i ) {
    int retReaderG, retG;
    bool errCodeFlagG = true;
    for ( int j = 0; j < times && errCodeFlagG; j += 1 ) {
        if ( rand() % 2 == 0 ) retReaderG = rtn1( i );
        else retReaderG = rtn2( i );

        if ( retReaderG != -1 ) {
            errCodeFlagG = false;
            retG = retReaderG;
        }
    }
    if ( errCodeFlagG && i % 2 ) {
        retReaderG = rtn2( i );
        if ( retReaderG != -1 ) {
            errCodeFlagG = false;
            retG = retReaderG;
        }
    }
    if ( errCodeFlagG ) {
        retReaderG = rtn1( i );
        if ( retReaderG != -1 ) {
            errCodeFlagG = false;
            retG = retReaderG;
        }
    }
    return errCodeFlagG ? -1 : retG;
}
int f( int i ) {
    int retReaderF, retF;
    bool loopFlagF = true;              //
    for ( int j = 0; j < times && loopFlagF; j += 1 ) {
        retReaderF = g( i );
        if ( retReaderF != -1 ) {
            loopFlagF = false;
            retF = retReaderF;
        }
    }
    if ( loopFlagF && i % 2 ) {
        retReaderF = g( i );
        if ( retReaderF != -1 ) {
            loopFlagF = false;
            retF = retReaderF;
        }
    }
    if ( loopFlagF ) {
        g( i );
        if ( retReaderF != -1 ) {
            loopFlagF = false;
            retF = retReaderF;
        }
    }
    return loopFlagF ? -1 : retF;
}
int main( int argc, char *argv[] ) {
    int seed = getpid();                // default value
    try {               // process command-line arguments
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
    srand( seed );			           // set random-number seed
    int rc = f( 3 );                          // begin program
    if ( rc == -1 )
        cout << "seed:" << seed << " times:" << times << " complete" << endl;
    else
        cout << "seed:" << seed << " times:" << times << " rc:" << rc << endl;
}
