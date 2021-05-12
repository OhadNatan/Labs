#include "newQ.h"
#include <conf.h>
#include <kernel.h>
#include <mem.h>

SYSCALL getmemForTime(int nbytes, int secs, void** ptr){
    (char*) ptr ;
    ptr = getmem(nbytes);
    if (ptr ==NULL)
        kprintf("no free memory to allocate");
        return(SYSERR);
    insertNewQ(nbytes, secs, void** ptr)
}