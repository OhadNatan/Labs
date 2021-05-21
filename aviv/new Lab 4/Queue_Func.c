#include "sleepQueue.h"
#include <conf.h>
#include <kernel.h>
#include <mem.h>

//Insert to the queue new temporery mem
SYSCALL insert2Queue(int nbytes, int sec, char **ptr)
{
    int i;
    int next;
    int prev;

    if (counterOfProc == 50)
    {
        kprintf("Queue is full\n");
        return 1;
    }

    if (counterOfProc == 0)
    {//start with enpty queue
        counterOfProc++;
        counter = 0;
        sleepHead = 0;
        //init new struct
        sleepQueue[sleepHead].size = nbytes;
        sleepQueue[sleepHead].next = -1;
        sleepQueue[sleepHead].secs = sec;
        sleepQueue[sleepHead].mptr = ptr;
        sleepQueuefree[0] = 1;
        return 0;
    }

    if (sec < sleepQueue[sleepHead].secs)
    { //insert to head of te queue
      
        //Finding the next free place at the queue
        for (i = 0; i < 50 && sleepQueuefree[i] != 0; i++);
        
        counterOfProc++;
        sleepQueue[sleepHead].secs -= sec;
        //init new struct
        sleepQueue[i].size = nbytes;
        sleepQueue[i].next = sleepHead;
        sleepQueue[i].secs = sec;
        sleepQueue[i].mptr = ptr;
        sleepQueuefree[i] = 1;
        sleepHead = i;
        return 0;
    }

    for (prev = sleepHead, next = sleepQueue[sleepHead].next; next == -1 || sleepQueue[next].secs < sec; prev = next, next = sleepQueue[next].next)
    {
        sec -= sleepQueue[prev].secs;
        if (next == -1)
            break;
    }

    counterOfProc++;

    //Finding the next free place at the queue
    for (i = 0; i < 50 && sleepQueuefree[i] != 0; i++);


    //init new struct
    sleepQueue[prev].next = i;
    sleepQueue[i].size = nbytes;
    sleepQueue[i].secs = sec;
    sleepQueue[i].mptr = ptr;



    if(next == -1)
        sleepQueue[i].next = -1;
    else{
        sleepQueue[i].next = next;
        sleepQueue[next].secs -= sec;
    }

    
    sleepQueuefree[i] = 1;
    return 0;
}

SYSCALL deleteMemFromQueue()
{//Delete the first mem from the queue, and free his memory block
    int toDelete;
    
    counterOfProc--;

    toDelete = sleepHead;

    sleepHead = sleepQueue[sleepHead].next;//new head for the queue
    sleepQueuefree[toDelete] = 0;//Set this index as free by init to 0

    freemem(*sleepQueue[toDelete].mptr, sleepQueue[toDelete].size);
    *sleepQueue[toDelete].mptr = NULL;
}

SYSCALL getmemForTime(int nbytes, int secs, void **ptr)
{
    int res;
    char *newMem = getmem(nbytes);
    *ptr = (void *)newMem;

    if (newMem == NULL)
    {
        kprintf("no free memory to allocate");
        return (SYSERR);
    }

    res = insertsleepQueue(nbytes, secs, (char **)ptr); 

    if (res == 1)
    {
        kprintf("no free memory to allocate");
        return (SYSERR);
    }

    return (OK);
}

SYSCALL freep(char* pblock){
    word bytes2Remove;
    struct	mblock	*p, *q;

    for ( q=&memlist, p=q->mnext ; (char *)p < pblock && (char *)p != NULL ; q=p, p=p->mnext );

    if((char *)p > pblock){
        bytes2Remove = (word)((char *)p - (char *)pblock);
        freemem(pblock, bytes2Remove);
    }
    else{
        kprintf("Error\n");
        return (SYSERR);
    }
    
    return (OK);
}