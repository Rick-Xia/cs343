#ifndef __Q2QUICKSORT_H__
#define __Q2QUICKSORT_H__

template<typename T> _Task Quicksort {
    unsigned int low;
    unsigned int high;
    unsigned int depth;
    int maxLen;
    T* array;

    unsigned int partition( unsigned int plow, unsigned int phigh );
    void qSortMultiDepth( unsigned int mlow, unsigned int mhigh );
    void qSortZeroDepth( unsigned int zlow, unsigned int zhigh );
    void main();
  public:
    Quicksort( T values[], unsigned int low, unsigned int high, unsigned int depth );
};

#endif

