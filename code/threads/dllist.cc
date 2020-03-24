#include "dllist.h"
#include "thread.h"
#include "copyright.h"
#include <stdio.h>

extern int kindIncorrect;

DLListElement::DLListElement( void *itemPtr, int sortKey )  // initialize a list element
{
    prev = next = NULL;
    item = itemPtr;
    key  = sortKey;
}
DLList::DLList()    // initialize the list
{
    first = last = NULL; 
}
DLList::~DLList()    // de-allocate the list
{
    while (Remove(NULL) != NULL);    // delete all the list elements
}

void DLList::Prepend(void *item)    // add to head of list (set key = min_key-1)
{
    DLListElement *element = new DLListElement(item, 0);
    if (IsEmpty())    // list is empty
    {
        first = element;
        last  = element;
    }
    else			  // else put it before first
    {
        element->key = first->key - 1;
        element->next = first;
        element->prev = NULL;
        first->prev = element;
        first = element;
    }
}

void DLList::Append(void *item)    // add to tail of list (set key = max_key+1)
{
    DLListElement *element = new DLListElement(item, 0);
    if (IsEmpty())    // list is empty
    {
        first = element;
        last  = element;
    }
    else			  // else put it after last
    {
        element->key = last->key + 1;
        element->next = NULL;
        element->prev = last;
        last->next = element;
        last = element;
    }
}

void *DLList::Remove(int *keyPtr)    // remove from head of list
{
    DLListElement *element = first;
    void *ptr;

    if (IsEmpty())  return NULL;

    ptr = first->item;
    if (first == last)    // list had one item before remove
    {
        first = NULL;
        last  = NULL;
    }
    else
    {
        first = element->next;
    }
    Print();//print the dllist

    *keyPtr = element->key;    // set *keyPtr to key of the removed item
    delete element;	   // deallocate list element -- no longer needed
    return ptr;    // return item (or NULL if list is empty)
}

bool DLList::IsEmpty()    // return true if list has elements
{
    return (first == NULL && last == NULL);
}

void DLList::SortedInsert(void *item, int sortKey)  // routines to put/get items on/off list in order (sorted by key)
{
    DLListElement *element = new DLListElement(item, sortKey);
    DLListElement *ptr;

    // printf("sortkey: %d\n", sortKey);

    if (IsEmpty())
    {
        first = element;
        last  = element;
    }
    else if (sortKey < first->key)     // item insert to the head of the list
    {
        // element->key = sortKey;
        element->next = first;
        element->prev = NULL;
        first->prev = element;
        first = element;
    }
    else    // find the right position
    {
        for (ptr = first; ptr != NULL; ptr = ptr->next)
        {
            if (sortKey < ptr->key)
            {
                // printf("%d %d\n", sortKey, ptr->key);
                // element->key = sortKey;
                element->next = ptr;
                element->prev = ptr->prev;
                (ptr->prev)->next = element;
                ptr->prev = element;
                ptr = element;
                break;
            }
        }
        if(ptr == NULL)    // item insert to the tail of the list
        {
            // element->key = sortKey;
            element->next = NULL;
            element->prev = last;
            last->next = element;
            last = element;
        }
    }
    Print();
}

void *DLList::SortedRemove(int sortKey)    // remove first item with key==sortKey
{
    // return NULL if no such item exists
    DLListElement *ptr;
    if (IsEmpty())  return NULL;
    ptr = first;
    while (ptr != NULL)
    {
        if (ptr->key != sortKey)
        {
            ptr = ptr->next;
        }
        else
        {
            if (ptr == first)
            {
                first->next = first;
                first->prev = NULL;
            }
            else if (ptr == last)
            {
                last->prev = last;
                last->next = NULL;
            }
            else
            {
                ptr->next->prev = ptr->prev;
                ptr->prev->next = ptr->next;
            }
        }
    }
    if (ptr == NULL)
        return NULL;
    else 
        return ptr->item;
}

void DLList::Print()    //print the dllist
{
    DLListElement *ptr;
    printf("Now DLList:");
    if (IsEmpty())
        printf(" NULL");
    else
    {
        for (ptr = first; ptr != NULL; ptr = ptr->next)
        {
            printf(" %d",ptr->key);
        }
    }
    printf("\n");
}
