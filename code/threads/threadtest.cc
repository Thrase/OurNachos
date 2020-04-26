// threadtest.cc
//	Simple test case for the threads assignment.
//
//	Create two threads, and have them context switch
//	back and forth between themselves by calling Thread::Yield,
//	to illustratethe inner workings of the thread system.
//
// Copyright (c) 1992-1993 The Regents of the University of California.
// All rights reserved.  See copyright.h for copyright notice and limitation
// of liability and disclaimer of warranty provisions.

#include <stdlib.h>
#include "copyright.h"
#include "system.h"
#include "dllist.h"
#include <time.h>
#include "synch.h"
#include "Table.h"
#include "BoundedBuffer.h"

// testNum is set in main.cc
int testNum = 2;
int threadNum = 2;
int n = 5;
int kindIncorrect = 0;
int bufferSizeTest = 5;


int isLocked;
int isTabled;
int isBuffered;

DLList *D;

extern void InsertItem(int n, DLList *D, int thread_num);
extern void RemoveItem(int n, DLList *D, int thread_num);
extern void TableInsert(int size, int which);
extern void TableRemove(int size, int which);
extern void BufferAdd(int size);
extern void BufferDelete(int size);

Lock *dllistLock=new Lock("dllist lock");
Condition *dllistConditon=new Condition("dllist condition");

//----------------------------------------------------------------------
// SimpleThread
// 	Loop 5 times, yielding the CPU to another ready thread
//	each iteration.
//
//	"which" is simply a number identifying the thread, for debugging
//	purposes.
//----------------------------------------------------------------------

void SimpleThread(int which)
{
    int num;

    for (num = 0; num < 5; num++)
    {
        printf("*** thread %d looped %d times\n", which, num);
        currentThread->Yield();
    }
}

void DLThread(int which)
{
    if(isLocked==1)
    {
        dllistLock->Acquire(); // acquire the lock
    }
    InsertItem(n, D, which);
    // error #1
    if (kindIncorrect == 1)
    {
        printf("[Error #1] Yield between InsertItem() and RemoveItem(). (threadtest.cc)\n");
        currentThread->Yield();
    }
    RemoveItem(n, D, which);
    if(isLocked==1)
    {
        dllistLock->Release(); // release the lock
    }
}

void TableThread(int which)
{
	if(isTabled == 1)
    {
        dllistLock->Acquire(); // acquire the lock
    }   
    TableInsert(n, which);
	currentThread->Yield();
	TableRemove(n, which);
    if(isTabled==1)
    {
        dllistLock->Release(); // release the lock
    }
}

void BufferThread(int which)
{
	if(which == 0)
    {
        BufferAdd(n);
    }
	else if(which==1)
    {
        BufferDelete(n);
    }
}

//----------------------------------------------------------------------
// ThreadTest1
// 	Set up a ping-pong between two threads, by forking a thread
//	to call SimpleThread, and then calling SimpleThread ourselves.
//----------------------------------------------------------------------

void ThreadTest1()
{
    DEBUG('t', "Entering ThreadTest1");

    Thread *t = new Thread("forked thread");

    t->Fork(SimpleThread, 1);
    SimpleThread(0);
}

void ThreadTestDL()
{
    DEBUG('t', "Entering ThreadTestDL");

    srand((unsigned)time(NULL));
    D = new DLList(); // global DLList for all threads to operate
    
    int i;
    for (i = 1; i < threadNum; i++)
    {
        Thread *t = new Thread("forked thread");
        t->Fork(DLThread, i); // fork other threads
    }
    DLThread(0);  // the first thread
}

void ThreadTestTable()
{
    DEBUG('t',"Entering ThreadTestTable\n");
    srand((unsigned)time(NULL));
    D = new DLList();

    int i;
    for (i = 1; i < threadNum; i++)
    {
        Thread *t = new Thread("forked thread");
        t->Fork(TableThread, i);
    }
    TableThread(0);
}

void ThreadTestBuffer()
{
    DEBUG('t',"Entering ThreadTestBuffer\n");
    srand((unsigned)time(NULL));

    Thread *t = new Thread("forked thread");
    t->Fork(BufferThread, 0);
    BufferThread(1);
}

//----------------------------------------------------------------------
// ThreadTest
// 	Invoke a test routine.
//----------------------------------------------------------------------

void ThreadTest()
{
    printf("===== parameters =====\n");
    printf("testNum        %d\n", testNum);
    printf("threadNum      %d\n", threadNum);
    printf("n              %d\n", n);
    printf("kindIncorrect  %d\n", kindIncorrect);
    printf("======================\n\n");

    switch (testNum)
    {
    case 1:
        ThreadTest1();
        break;
    case 2:
        printf("please input 0 or 1(0-unlocked,1-locked):");
	    scanf("%d",&isLocked);
        ThreadTestDL(); // start Thread for testing
        break;
    case 3:
        printf("please input 0 or 1(0-unlocked,1-locked):");
	    scanf("%d",&isTabled);
	    ThreadTestTable();
        break;
    case 4:
        printf("please input 0 or 1(0-unlocked,1-locked):");
        scanf("%d",&isBuffered);
        ThreadTestBuffer();
        break;
    default:
        printf("No test specified.\n");
        break;
    }
}
