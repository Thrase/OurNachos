// synch.cc 
//	Routines for synchronizing threads.  Three kinds of
//	synchronization routines are defined here: semaphores, locks 
//   	and condition variables (the implementation of the last two
//	are left to the reader).
//
// Any implementation of a synchronization routine needs some
// primitive atomic operation.  We assume Nachos is running on
// a uniprocessor, and thus atomicity can be provided by
// turning off interrupts.  While interrupts are disabled, no
// context switch can occur, and thus the current thread is guaranteed
// to hold the CPU throughout, until interrupts are reenabled.
//
// Because some of these routines might be called with interrupts
// already disabled (Semaphore::V for one), instead of turning
// on interrupts at the end of the atomic operation, we always simply
// re-set the interrupt state back to its original value (whether
// that be disabled or enabled).
//
// Copyright (c) 1992-1993 The Regents of the University of California.
// All rights reserved.  See copyright.h for copyright notice and limitation 
// of liability and disclaimer of warranty provisions.

#include "copyright.h"
#include "synch.h"
#include "system.h"

//----------------------------------------------------------------------
// Semaphore::Semaphore
// 	Initialize a semaphore, so that it can be used for synchronization.
//
//	"debugName" is an arbitrary name, useful for debugging.
//	"initialValue" is the initial value of the semaphore.
//----------------------------------------------------------------------

Semaphore::Semaphore(char* debugName, int initialValue)
{
    name = debugName;
    value = initialValue;
    queue = new List;
}

//----------------------------------------------------------------------
// Semaphore::Semaphore
// 	De-allocate semaphore, when no longer needed.  Assume no one
//	is still waiting on the semaphore!
//----------------------------------------------------------------------

Semaphore::~Semaphore()
{
    delete queue;
}

//----------------------------------------------------------------------
// Semaphore::P
// 	Wait until semaphore value > 0, then decrement.  Checking the
//	value and decrementing must be done atomically, so we
//	need to disable interrupts before checking the value.
//
//	Note that Thread::Sleep assumes that interrupts are disabled
//	when it is called.
//----------------------------------------------------------------------

void
Semaphore::P()
{
    IntStatus oldLevel = interrupt->SetLevel(IntOff);	// disable interrupts
    
    while (value == 0) { 			// semaphore not available
	queue->Append((void *)currentThread);	// so go to sleep
	currentThread->Sleep();
    } 
    value--; 					// semaphore available, 
						// consume its value
    
    (void) interrupt->SetLevel(oldLevel);	// re-enable interrupts
}

//----------------------------------------------------------------------
// Semaphore::V
// 	Increment semaphore value, waking up a waiter if necessary.
//	As with P(), this operation must be atomic, so we need to disable
//	interrupts.  Scheduler::ReadyToRun() assumes that threads
//	are disabled when it is called.
//----------------------------------------------------------------------

void
Semaphore::V()
{
    Thread *thread;
    IntStatus oldLevel = interrupt->SetLevel(IntOff);

    thread = (Thread *)queue->Remove();
    if (thread != NULL)	   // make thread ready, consuming the V immediately
	scheduler->ReadyToRun(thread);
    value++;
    (void) interrupt->SetLevel(oldLevel);
}

// Dummy functions -- so we can compile our later assignments 
// Note -- without a correct implementation of Condition::Wait(), 
// the test case in the network assignment won't work!
Lock::Lock(char* debugName) 
{
    name=debugName;     // just the same..
    holder = new Thread("holder");
    lockFlag = FALSE;   // bool.h defines FALSE.
    lockQueue = new List();   // just the same..
}

Lock::~Lock() 
{
    delete lockQueue;       // just the same
    delete holder;
}

void Lock::Acquire() 
{
    ASSERT(!isHeldByCurrentThread());   // illegal check.
    IntStatus oldLevel = interrupt->SetLevel(IntOff);	// disable interrupts
    while(lockFlag == TRUE)
    {
        // still sleep
        lockQueue->Append(currentThread);
        currentThread->Sleep();
    }
    // now leaving
    /*
        1. lock
        2. tell user
    */
    lockFlag=TRUE;
    holder = currentThread;
    // print: current Thread name, lock name (for debugging)
    printf("Thread %s acquires lock %s\n", currentThread->getName(), name);
        
    (void) interrupt->SetLevel(oldLevel);	// re-enable interrupts
}

void Lock::Release() 
{
    ASSERT(isHeldByCurrentThread());   // illegal check

    IntStatus oldLevel = interrupt->SetLevel(IntOff);	// disable interrupts
    
    Thread* leavingThread = (Thread*)lockQueue -> Remove();  // void* -> Thread*
    if (leavingThread!=NULL)    // make thread ready
    {
        scheduler->ReadyToRun(leavingThread);       // just the same..
    }

    /*
        NOTE:
        when release lock, the order should be 
        different (upside-down) from acquire.
    */
    // print: current Thread name, lock name (for debugging)
    printf("Thread %s releases lock %s\n", currentThread->getName(), name);
    holder = NULL;  // holder given to NULL (mesa method)
    lockFlag = FALSE;

    (void) interrupt->SetLevel(oldLevel);	// re-enable interrupts
}

bool Lock::isHeldByCurrentThread()
{
    if (holder==currentThread && lockFlag)
    {
        return TRUE;
    }
    return FALSE;
}

Condition::Condition(char* debugName) 
{
    name = debugName;
    conditionQueue = new List;
}

Condition::~Condition() 
{
    delete conditionQueue;
}

void Condition::Wait(Lock* conditionLock) 
{
    // wait for.
    // ASSERT(FALSE); 
    ASSERT(conditionLock->isHeldByCurrentThread());     // check illegal: if it's held.
    IntStatus oldLevel = interrupt->SetLevel(IntOff);	// disable interrupts
    
    conditionLock->Release();
    conditionQueue->Append(currentThread);   //block. Sleep with Int Off won't make problems.
    currentThread->Sleep();
    conditionLock->Acquire();

    (void) interrupt->SetLevel(oldLevel);	// re-enable interrupts
}

void Condition::Signal(Lock* conditionLock) 
{
    ASSERT(conditionLock->isHeldByCurrentThread());
    IntStatus oldLevel = interrupt->SetLevel(IntOff);	// disable interrupts

    Thread* leavingThread = (Thread*)conditionQueue -> Remove();  // void* -> Thread*
    if (leavingThread!=NULL)
    {
        scheduler->ReadyToRun(leavingThread);
    }
    // same as Lock.

    (void) interrupt->SetLevel(oldLevel);	// re-enable interrupts
}

void Condition::Broadcast(Lock* conditionLock) 
{
    ASSERT(conditionLock->isHeldByCurrentThread());
    IntStatus oldLevel = interrupt->SetLevel(IntOff);	// disable interrupts

    Thread* leavingThread = (Thread*)conditionQueue -> Remove();  // void* -> Thread*
    while (leavingThread!=NULL)
    {
        // awake all.
        scheduler->ReadyToRun(leavingThread);
        leavingThread = (Thread*)conditionQueue -> Remove();  // void* -> Thread*
    }
    // same as Lock.

    (void) interrupt->SetLevel(oldLevel);	// re-enable interrupts
}
