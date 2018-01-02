#include <iostream>
#include <cstdlib>
using namespace std;
#include <unistd.h>                     // access: getpid
#include <setjmp.h>                     // access: setjmp longjmp

#ifdef NOOUTPUT
#define PRT( stmt )
#else
#define PRT( stmt ) stmt
#endif // NOOUTPUT

jmp_buf *buf;

PRT( struct T { ~T() { cout << "~"; } }; )

struct E {};                            // exception type
long int freq = 5;                      // exception frequency

long int Ackermann( long int m, long int n ) {
    PRT( T t; )
    if ( m == 0 ) {
        if ( rand() % freq == 0 ) longjmp(*buf, 1);
        return n + 1;
    } else if ( n == 0 ) {
        jmp_buf localbuf;
        jmp_buf *oldbuf = buf;
        buf = &localbuf;
        if ( !setjmp(localbuf) ) {
            int temp = Ackermann( m - 1, 1 );
            return temp;
        } else {
            PRT( cout << "E1 " << m << " " << n << endl );
            buf = oldbuf;
            if ( rand() % freq == 0 ) longjmp(*buf, 1);
        } // if
    } else {
        jmp_buf localbuf;
        buf = &localbuf;
        if ( !setjmp(localbuf) ) {
            int temp = Ackermann( m, n - 1 );
            buf = &localbuf;
            int ret = Ackermann( m - 1, temp);
            return ret;
        } else {
            PRT( cout << "E2 " << m << " " << n << endl );
        } // if
    } // if
    return 0;                           // recover by returning 0
}
int main( int argc, const char *argv[] ) {
    long int Ackermann( long int m, long int n );
    long int m = 4, n = 6, seed = getpid(); // default values
    try {               // process command-line arguments
        switch ( argc ) {
          case 5: freq = stoi( argv[4] ); if ( freq <= 0 ) throw 1;
          case 4: seed = stoi( argv[3] ); if ( seed <= 0 ) throw 1;
          case 3: n = stoi( argv[2] ); if ( n < 0 ) throw 1;
          case 2: m = stoi( argv[1] ); if ( m < 0 ) throw 1;
          case 1: break;                // use defaults
          default: throw 1;
        } // switch
    } catch( ... ) {
        cout << "Usage: " << argv[0] << " [ m (> 0) [ n (> 0) [ seed (> 0)"
            " [ freq (> 0) ] ] ] ]" << endl;
        exit( 1 );
    } // try
    srand( seed );          // seed random number
    // begin program
    PRT( cout << m << " " << n << " " << seed << " " << freq << endl );
    jmp_buf localbuf;
    buf = &localbuf;
    if ( !setjmp(localbuf) ) {
        long int val = Ackermann( m, n );
        buf = &localbuf;
        PRT( cout << val << endl );
    }
    else PRT( cout << "E3" << endl );
}
