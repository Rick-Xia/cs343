#ifndef __AUTOMATICSIGNAL_H__
#define __AUTOMATICSIGNAL_H__

#define AUTOMATIC_SIGNAL uCondition autoSignalCond

#define WAITUNTIL( pred, before, after ) \
    if ( !pred ) { \
        before; \
        for ( ;; ) { \
          if ( pred ) break; \
            autoSignalCond.wait(); \
        } \
        after; \
    }

#define RETURN( expr... ) \
    if ( !autoSignalCond.empty() ) autoSignalCond.signal(); \
    return expr;

#endif
