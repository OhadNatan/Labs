
#include <butler.h>
#include <conf.h>
#include <kernel.h>
#include <proc.h>
#include <q.h>
#include <sleep.h>
#include <io.h>
#include "lab3H.h"

int calcPrio(int pid)//calc the effec prio
{
    int prio;

    if (pid<4)//check if is a legal proc
        return pprio[pid];

    if(runnable_time[pid]==0)//check that we didnt div in 0
        return 1+pprio[pid];
    

    prio = 1 + (pprio[pid] * ((runnable_time[pid] - current_time[pid])/((double) runnable_time[pid])));
    //calc the effectiv priority
    if (prio<1) return 1;
    
    //never get up to the butler priority
    if (prio>BTLRPRIO-1) return BTLRPRIO-1;

    return prio;

}

int findMaxPrio() // return the max prio from the proc`s in the ready Q
{
    int pidCurr,maxPrio=0;
    pidCurr = q[rdyhead].qnext;
  	while (q[pidCurr].qkey < MAXINT){	/* tail has MAXINT as key	*/
		  if (q[pidCurr].qkey >= maxPrio)
        maxPrio = q[pidCurr].qkey;
      pidCurr = q[pidCurr].qnext;
    }
    return maxPrio;
}

int findMaxPrioPid() // return the pid of the proc with the max prio in the ready Q
{
    int MaxPrioPid,pidCurr,maxPrio=0;
    pidCurr = q[rdyhead].qnext;
  	while (q[pidCurr].qkey < MAXINT){	/* tail has MAXINT as key	*/
		  if (q[pidCurr].qkey >= maxPrio){
        maxPrio = q[pidCurr].qkey;
        MaxPrioPid = pidCurr;
      }
      pidCurr = q[pidCurr].qnext;
    }
    return MaxPrioPid;
}