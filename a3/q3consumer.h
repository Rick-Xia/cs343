#ifndef __Q3CONSUMER_H__
#define __Q3CONSUMER_H__

#include "q3buffer.h"

_Task Consumer {
    BoundedBuffer<int> &buffer;
    const int Delay;
    const int Sentinel;
    int& sum;

    void main();
  public:
    Consumer( BoundedBuffer<int> &buffer, const int Delay, const int Sentinel, int &sum );
};

#endif

