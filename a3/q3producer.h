#ifndef __Q3PRODUCER_H__
#define __Q3PRODUCER_H__

#include "q3buffer.h"

_Task Producer {
    BoundedBuffer<int>& buffer;
    const int Produce;
    const	int Delay;

    void main();
  public:
    Producer( BoundedBuffer<int> &buffer, const int Produce, const int Delay );
};

#endif

