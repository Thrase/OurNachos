#include "synch.h"

class BoundedBuffer {
   public:
     // create a bounded buffer with a limit of 'maxsize' bytes
     BoundedBuffer(int maxsize);
     ~BoundedBuffer();
     // read 'size' bytes from the bounded buffer, storing into 'data'.
     // ('size' may be greater than 'maxsize')
     void Read(void *data, int size);

     // write 'size' bytes from 'data' into the bounded buffer.
     // ('size' may be greater than 'maxsize')
     void Write(void *data, int size);
   private:
	 int bufferSize; //size of buffer
	 int *Buffer;
	 int writeIn;   //the position for writing
	 int readOut;   //the position for reading
	 Semaphore *s;  //for mutual exclusion
	 Semaphore *n;  //if there is a data in buffer for reading
	 Semaphore *e;  //if there is a position in buffer for writing
};

