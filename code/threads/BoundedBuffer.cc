#include <stdlib.h>
#include "BoundedBuffer.h"
#include "system.h"
extern int isBuffered;

BoundedBuffer::BoundedBuffer(int maxsize)
{
	bufferSize = maxsize;
	Buffer = new int[maxsize];
	for(int i=0;i<maxsize;i++) // Initialize the buffer
		Buffer[i] = NULL;
	writeIn = 0;
	readOut = 0;
	s=new Semaphore("s",1);
	n=new Semaphore("n",0);
	e=new Semaphore("e",bufferSize);
}

BoundedBuffer::~BoundedBuffer()
{
	free(Buffer);
	delete s;
	delete n;
	delete e;
}

void BoundedBuffer::Read(void *data,int size) // like cosumer
{
	int *readData = (int*)data;
	while(size--) // only read one data once
	{
		if(isBuffered == 1)
		{
			while(writeIn == readOut) // cache is empty, waite for writting
				n->P();
			s->P();
		}
		*readData = *readData + Buffer[readOut];
		printf("Thread %s read %d\n",currentThread -> getName(),Buffer[readOut]);
		readOut = (readOut + 1) % bufferSize;
		if(isBuffered == 1)
		{
			s->V();
			e->V();
		}
	}
}

void BoundedBuffer::Write(void *data,int size) // like producer
{
	int *writeData = (int*)data;
	while(size--) // only write one data once
	{
		if(isBuffered == 1)
		{
			while((writeIn + 1) % bufferSize == readOut) // cache is full, wait for the empty bit
				e->P();
			s->P();
		}
		Buffer[writeIn] = *writeData++;
		printf("Thread %s write %d\n",currentThread -> getName(),Buffer[writeIn]);
		writeIn = (writeIn + 1) % bufferSize;
		if(isBuffered == 1)
		{
			s->V();
			n->V();
		}
	}
}
