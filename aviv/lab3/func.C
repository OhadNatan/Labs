#include "lab3H.h"
#include <butler.h>


int calcPrio(int pid)
{
    int prio;

    if (pid<4)//check if is a legal proc
        return pprio[pid];

    if(runnable_time[pid]==0)//check that we didnt div in 0
        return 1+pprio[pid];
    
    //calc the effectiv priority
    prio = 1+ (pprio[pid] * (runnable_time[pid] - current_time[pid]))/runnable_time[pid];

    if (prio<1) return 1;
    
    //never get up to the butler priority
    if (prio>BTLRPRIO-1) return BTLRPRIO-1;

    return prio;

}