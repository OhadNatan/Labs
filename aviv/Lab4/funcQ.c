#include "newQ.h"

int insertNewQ(int nbytes, int sec, void** ptr){ //insert to the newQ a new temp mem block
   int i,prev,next;
    if (counterInQ==0){
        headNewQ = 0;
        newQ[headNewQ].size = nbytes;
        newQ[headNewQ].next = -1;
        newQ[headNewQ].secs = sec;
        newQ[headNewQ].mptr =(char*)ptr;
        newQfree[0]=1;
        counterInQ++;
        return 1;
    }

    if (counterInQ ==50){
        kprintf("the Queue is full ,try again leter ");
        return -1;
    }
    if ( sec < newQ[headNewQ].secs){
        for(i=0;i<50;i++)
            if(newQfree[i]==0)
                break;
        newQ[headNewQ].secs -= sec;
        newQ[i].size = nbytes;
        newQ[i].next = headNewQ;
        newQ[i].secs = sec;
        newQ[i].mptr =(char*)ptr;
        newQfree[i]=1;
        headNewQ = i;
        counterInQ++; 
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
    newQ[i].mptr =(char*)ptr;
    newQ[next].secs -= sec;
    newQfree[i]=1;
    counterInQ++; 
    return 1;
}