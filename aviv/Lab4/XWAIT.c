#include <conf.h>
#include <kernel.h>
#include <proc.h>
#include <q.h>
#include "FSONS.h"

SYSCALL xwait() {  
    if(sonsCounter[currpid] == 0)
        return;
    xwaitFlag[currpid] = 1;
    suspend(currpid);
}


SYSCALL xwaitAll() {  
    if(sonsCounter[currpid] == 0)
        return;
    xwaitAllFlag[currpid] = 1;
    suspend(currpid);
}