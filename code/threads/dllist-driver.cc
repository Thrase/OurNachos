/*
The third file should contain two functions, 
one of which generates N items with random keys 
(or to aid debugging, you might control the input sequence with a more carefully selected key order) 
    and inserts them into a doubly-linked list, 
and the other which removes N items starting from the head of the list and prints out the removed items to the console. 
Both functions should take as arguments the integer N and a pointer to the list.
*/

#include <stdlib.h>
#include "dllist.h"
#include "time.h"
#include "thread.h"
#include <stdio.h>

extern int kindIncorrect;
extern Thread *currentThread;

void InsertItem(int n, DLList *D, int thread_num)
{
    int i, val;
    srand(time(NULL));
    for (i = 0; i < n; i++)
    {
        val = rand() % 100 + 1; // insert random number
        // val = i;
        D->SortedInsert(NULL, val);
        printf("Thread %d inserted item with key=%d\n\n", thread_num, val);

        // Error #2
        if (kindIncorrect == 2)
        {
            printf("[Error #2] Yield after SortedInsert(). (dllist-driver.cc)\n");
            currentThread->Yield();
        }
    }
}

void RemoveItem(int n, DLList *D, int thread_num)
{
    int i, val;
    for (i = 0; i < n; i++)
    {
        D->Remove(&val);
        printf("Thread %d removed item whose key=%d\n\n", thread_num, val);

        // Error #3
        if (kindIncorrect == 3)
        {
            printf("[Error #3] Yield after Remove(). (dllist-driver.cc)\n");
            currentThread->Yield();
        }
    }
    if (D->IsEmpty())
        printf("List empty. Thread %d finished removing.\n\n", thread_num);
}
