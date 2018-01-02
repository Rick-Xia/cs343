#ifndef __Q3BUFFER_H__
#define __Q3BUFFER_H__

template<typename T>
class BoundedBuffer {
   const unsigned int size;             // Buffer maximun size

   T* internalBuffer;                   // The Instance of the buffer, which I am using an array here
   unsigned int numOfStorage = 0;       // The number of spots filled, making length check cheaper
   int consumerIndex = -1;              // An index for where to retrieve data,
                                        //  which it should always be the next slot
   int producerIndex = 0;               // An index for where to insert data,
                                        //  which it should always be the current slot

   bool isBufferEmpty();                // Helper to check if the buffer is empty
   bool isBufferFull();                 // helper to check if the buffer is full
   T bufferPop();                       // Helper to retrieve the next data
   void bufferPush( T elem );           // Helper to insert a data

   uOwnerLock curOwnerLock;
   uCondLock insertionLock;
   uCondLock removalLock;

#ifdef NOBUSY
   uCondLock bargeLock;
   bool bargeFlag = false;              // Flag variable for preventing barging
   void bargeCheck();                   // Barging avoidance: 
                                        //  Helper to check for barging and put the barging thread to sleep
#endif

  public:
    ~BoundedBuffer();
    BoundedBuffer( const unsigned int size = 10 );
    void insert( T elem );
    T remove();
};

#endif

