#include <stdlib.h>
#include "system.h"
#include "BoundedBuffer.h"

BoundedBuffer *buffTest=new BoundedBuffer(4);
void BufferAdd(int size)
{
    int *data = new int[size];
	for (int i=0; i<size; i++)
	{
		data[i] = rand() % 100 + 1;
	}
	buffTest -> Write(data,size);
}

void BufferDelete(int size)
{
	int *data = new int[size];
	buffTest -> Read(data,size);
}
