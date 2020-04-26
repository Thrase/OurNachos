#include <stdlib.h>
#include "Table.h"
#include "time.h"
#include "thread.h"
#include <stdio.h>

int ind[10];
Table *tableTest = new Table(10);
void TableInsert(int n, int thread_num)
{
    int i;
    for (i = 0; i < n; i++)
    {
        void *val = (void *)(rand() % 100 + 1); // insert random number
        ind[i] = tableTest->Alloc(val);
        printf("Thread %d inserted node with key=%d\n", thread_num, int(val));
		printf("Table[%d]'s node is %d\n\n", ind[i], int(val));

    }
}

void TableRemove(int n, int thread_num)
{
    int i, val;
    for (i = 0; i < n; i++)
    {
        val = (int) tableTest->Get(ind[i]);
        tableTest->Release(ind[i]);
        printf("Thread %d removed node whose key=%d\n\n", thread_num, val);

    }
    if (tableTest->IsEmpty())
        printf("Table is empty. Thread %d finished removing.\n\n", thread_num);
}
