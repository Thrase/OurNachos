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

// testNum is set in main.cc
int testNum = 2;
int threadNum = 2;
int n = 5;
int kindIncorrect = 0;

DLList *D;

extern void InsertItem(int n, DLList *D, int thread_num);
extern void RemoveItem(int n, DLList *D, int thread_num);

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

void DLTestThread(int which)
{
    InsertItem(n, D, which);

    // error #1
    if (kindIncorrect == 1)
    {
        printf("[Error #1] Yield between InsertItem() and RemoveItem(). (threadtest.cc)\n");
        currentThread->Yield();
    }
    RemoveItem(n, D, which);
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

    D = new DLList(); // global DLList for all threads to operate
    DLTestThread(0);  // the first thread

    int i;
    for (i = 1; i < threadNum; i++)
    {
        Thread *t = new Thread("forked thread of DLTestThread");
        t->Fork(DLTestThread, i); // fork other threads
    }
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
        ThreadTestDL(); // start Thread for testing
        break;
    default:
        printf("No test specified.\n");
        break;
    }
}
