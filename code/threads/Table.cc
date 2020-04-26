#include <stdio.h>
#include <stdlib.h>
#include "Table.h"
#include "synch.h"
#include "synch-sem.h"


extern bool isTabled;

Table::Table(int size)
{
    tableSize = size;
    lock = new Lock("Table lock.");
    table = new int[size];
    for (int i = 0; i < size; i++) {
        table[i] = NULL;
    }
}

Table::~Table()
{
    delete lock;
    free(table);
}

int Table::Alloc(void *object)
{
    if (isTabled == true) {
        lock -> Acquire();
    }

    int tmp = -1;

    for (int i = 0; i < tableSize; i++){
        if(table[i] == NULL) {
            table[i] = (int)object; 
            tmp = i;
            break;
        }
    }

    if (isTabled == true) {
        lock -> Release();
    }

    return tmp;
}

int *Table::Get(int index)
{
    if (isTabled == true)
    {
        lock -> Acquire();
    }

    int *i;

    if (index < tableSize)
    {
        i =(int *)table[index];
    }
    else
    {
        i =(int *) NULL;
    }

    if (isTabled == true)
    {
        lock -> Release();
    }

    return i;
}

void Table::Release(int index)
{
    if (isTabled == true)
    {
        lock -> Acquire();
    }

    if (index < tableSize)
    {
        if (table[index] != NULL)
        {
            table[index] = NULL;
        }
        else
        {
             printf(" Table[%d] is an empty node. \n", index);
        }
    }
    else
    {
        printf(" Table[%d] does not exist. \n", index);
    }
    

    if (isTabled == true)
    {
        lock -> Release();
    }
}

bool Table::IsEmpty()
{
    int num = 0;
    bool tmp;
    if (isTabled == true)
    {
        lock -> Acquire();
    }

    for (int i=0; i < tableSize; i++)
    {
        if (table[i] != NULL)
        {
            num ++;
        }
    }

    if( num == 0)
    {
        tmp = true;
    }
    else
    {
        tmp = false;
    }

    if (isTabled == true)
    {
        lock -> Release();
    }

    return tmp;
}