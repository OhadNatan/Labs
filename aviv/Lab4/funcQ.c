#include "newQ.h"
#include <conf.h>
#include <kernel.h>

SYSCALL insertNewQ(int nbytes, int sec, char* ptr){ //insert to the newQ a new temp mem block
   int i,prev,next;

   if (counterInQ ==50){
        kprintf("the Queue is full ,try again leter ");
        return -1;
    }

    if (counterInQ==0){  //start list from empthy
        timer = 0; //for clkint
        headNewQ = 0;
        newQ[headNewQ].size = nbytes;
        newQ[headNewQ].next = -1;
        newQ[headNewQ].secs = sec;
        newQ[headNewQ].mptr = ptr;
        newQfree[0]=1;
        counterInQ++;
         kprintf("ama shel ohad is not free");
        return 1;
    }

    
    if ( sec < newQ[headNewQ].secs){  //insert to head 
        for(i=0;i<50;i++)
            if(newQfree[i]==0)
                break;
        newQ[headNewQ].secs -= sec;
        newQ[i].size = nbytes;
        newQ[i].next = headNewQ;
        newQ[i].secs = sec;
        newQ[i].mptr =ptr;
        newQfree[i]=1;
        headNewQ = i;
        counterInQ++;
         kprintf("ama shel ohad is  not free"); 

        return 1;
    }
 
    for( prev=headNewQ ,next = newQ[headNewQ].next  ; newQ[next].secs < sec ;prev=next ,next = newQ[next].next )
		sec -= newQ[i].secs;

    for(i=0;i<50;i++)
        if(newQfree[i]==0)
            break;

    newQ[prev].next = i;
    newQ[i].size = nbytes;
    newQ[i].next = next;
    newQ[i].secs = sec;
    newQ[i].mptr = ptr;
    newQ[next].secs -= sec;
    newQfree[i]=1;
    counterInQ++;
     kprintf("ama shel ohad is not free"); 
    return 1;
}

SYSCALL deleteFromQ(){
    int toDelete = headNewQ;
    headNewQ = newQ[headNewQ].next;
    newQfree[toDelete]=0;
    freemem(newQ[toDelete].mptr, newQ[toDelete].size);
    newQ[toDelete].mptr = NULL;
    kprintf("ama shel ohad is free");
}

SYSCALL getmemForTime(int nbytes, int secs, void** ptr){ 
    
    (char*)ptr = getmem(nbytes);
    if (ptr ==NULL){
        kprintf("no free memory to allocate");
        return(SYSERR);
    }
    if(insertNewQ(nbytes, secs, (char*)ptr)==-1){
        kprintf("no free memory to allocate");
        return(SYSERR);
    }
    return (OK);
}