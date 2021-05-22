#include <conf.h>
#include <kernel.h>
#include <proc.h>
#include <q.h>
#include "FSONS.h"

SYSCALL xwait() {  
    if(numOfSons[currpid] == 0)
        return;
    XWAITflag[currpid] = 1;
    suspend(currpid);
}


SYSCALL xwaitAll() {  
    if(numOfSons[currpid] == 0)
        return;
    XWaitAllFlag[currpid] = 1;
    suspend(currpid);
}