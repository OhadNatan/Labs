#include "newQ.h"
#include <conf.h>
#include <kernel.h>
#include <mem.h>

//Insert to the queue new temporery mem
SYSCALL insert2Queue(int nbytes, int sec, char **ptr)
{
    int i;
    int next;
    int prev;

    if (counterInQ == 50)
    {
        kprintf("Queue is full\n");
        return 1;
    }

    if (counterInQ == 0)
    {//start with enpty queue
        timer = 0; //for clkint
        headNewQ = 0;
        newQ[headNewQ].size = nbytes;
        newQ[headNewQ].next = -1;
        newQ[headNewQ].secs = sec;
        newQ[headNewQ].mptr = ptr;
        newQfree[0] = 1;
        counterInQ++;
        return 0;
    }

    if (sec < newQ[headNewQ].secs)
    { //insert to head
        for (i = 0; i < 50; i++)
            if (newQfree[i] == 0)
                break;
        newQ[headNewQ].secs -= sec;
        newQ[i].size = nbytes;
        newQ[i].next = headNewQ;
        newQ[i].secs = sec;
        newQ[i].mptr = ptr;
        newQfree[i] = 1;
        headNewQ = i;
        counterInQ++;
        return 0;
    }

    for (prev = headNewQ, next = newQ[headNewQ].next; next == -1 || newQ[next].secs < sec; prev = next, next = newQ[next].next)
    {
        sec -= newQ[prev].secs;
        if (next == -1)
            break;
    }

    //Finding the next free place at the queue
    for (i = 0; i < 50 && newQfree[i] != 0; i++);

    newQ[prev].next = i;
    newQ[i].size = nbytes;
    newQ[i].secs = sec;
    newQ[i].mptr = ptr;
    if (next != -1)
    {
        newQ[i].next = next;
        newQ[next].secs -= sec;
    }
    else
        newQ[i].next = -1;
    newQfree[i] = 1;
    counterInQ++;
    return 0;
}

SYSCALL deleteFromQ()
{
    int toDelete = headNewQ;
    headNewQ = newQ[headNewQ].next;
    newQfree[toDelete] = 0;
    freemem(*newQ[toDelete].mptr, newQ[toDelete].size);
    *newQ[toDelete].mptr = NULL;
    counterInQ--;
}

SYSCALL getmemForTime(int nbytes, int secs, void **ptr)
{

    char *send = getmem(nbytes);
    *ptr = (void *)send;
    if (send == NULL)
    {
        kprintf("no free memory to allocate");
        return (SYSERR);
    }
    if (insertNewQ(nbytes, secs, (char **)ptr) == -1)
    {
        kprintf("no free memory to allocate");
        return (SYSERR);
    }
    return (OK);
}

SYSCALL freep(char* pblock){
    word nbytesToDel;
    struct	mblock	*p, *q;
    for ( q=&memlist, p=q->mnext ; (char *)p < pblock && (char *)p != NULL ; q=p, p=p->mnext );
    if((char *)p > pblock)
        nbytesToDel = (word)((char *)p - (char *)pblock); 
    else{
        kprintf("\n***Error***\n");
        return (SYSERR);
    }
    freemem(pblock, nbytesToDel);
    return (OK);
}