
#include <butler.h>
#include <conf.h>
#include <kernel.h>
#include <proc.h>
#include <q.h>
#include "lab3H.h"

int calcPrio(int pid)//calc the effec prio
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

int findMaxPrio() // return the max prio from the proc`s in the ready Q
{
    int pidCurr,maxPrio=0;
    pidCurr = q[rdyhead].qnext;
    maxPrio = q[pidCurr].qkey;
    while(q[pidCurr].qnext!=-1)
    {
      if (q[pidCurr].qkey > maxPrio)  maxPrio = q[pidCurr].qkey;
      pidCurr = q[pidCurr].qnext;
    }
    return maxPrio;
}

int findMaxPrioPid() // return the pid of the proc with the max prio in the ready Q
{
    int MaxPrioPid,pidCurr,maxPrio=0;
    pidCurr = q[rdyhead].qnext;
    maxPrio = q[pidCurr].qkey;
    if(q[pidCurr].qnext == -1)
      return pidCurr;
    while(q[pidCurr].qnext!=-1) 
    {
      if (q[pidCurr].qkey > maxPrio){
        maxPrio = q[pidCurr].qkey;
        MaxPrioPid = pidCurr;
      }
      pidCurr = q[pidCurr].qnext;
    }
    return MaxPrioPid;
}