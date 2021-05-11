#include <conf.h>
#include <kernel.h>
#include <proc.h>
#include "SONG.H"



int insertson(int father, int son) {
    struct sons* pson;
    struct sonsquery* soq;
    pson = &procson[father];
    if (pson->quantity >= NPROC) 
        return -1;
    
   
    if (pson->quantity == 0) {
        pson->first = son;
        pson->last = son;
        soq = &(pson->soq[son]);
        soq->next = -1;
    }
    else {
        soq = &(pson->soq[pson->last]);
        soq->next = son;
        pson->last = son;
        soq = &(pson->soq[pson->last]);
        soq->next = -1;
    }
    pson->quantity++;
    return 0;
}

int getfirstson(int pid){
    struct sons* pson;
    struct sonsquery* soq;
    int firstson;
    
    pson = &procson[pid];
    if (pson->quantity <= 0)
        return -1;
    else {
        firstson = pson->first;
        if(pson->quantity == 1){
            soq = &(pson->soq[firstson]);
            pson->first = pson->last = -1;
            
        }
        else {
            soq = &(pson->soq[firstson]);
            pson->first = soq->next;
        }
        pson->quantity--;
        return firstson;
    }
   
}
//first son pid by given PID
int firstsonpid(int pid) {
    return procson[pid].quantity <= 0 ? -1 : procson[pid].first;
}