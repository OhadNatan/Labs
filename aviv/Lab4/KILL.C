/* kill.c - kill */
/* 8086 version */

#include <conf.h>
#include <kernel.h>
#include <proc.h>
#include <sem.h>
#include <mem.h>
#include <q.h>
#include <sleep.h>
#include "FSONS.h"

/*------------------------------------------------------------------------
 *  kill  --  kill a process and remove it from the system
 *------------------------------------------------------------------------
 */
SYSCALL kill(pid)
	int	pid;		     /* process to kill		*/
{
	struct	pentry	*pptr;	     /* points to proc. table for pid */
	int	ps;	             /* saved processor status	*/
        int pstate;
	int i, Fpid;

	disable(ps);
	if (isbadpid(pid) || (pptr = &proctab[pid])->pstate==PRFREE) {
		restore(ps);
		return(SYSERR);
	}
	if (--numproc == 0)
		xdone();

		//----------------------Lab4 changes------------------------------------------------------------
	Fpid = fatherPids[pid];
	sonsCounter[Fpid]--; //update total num of son of the father process
	if(firstSons[Fpid] == pid){ //check if the pid is first son of process
		if(xwaitFlag[Fpid] == 1){
			xwaitFlag[Fpid] = 0;
			resume(Fpid);
		}
	}
	if(sonsCounter[Fpid] == 0){
		firstSons[Fpid] = 0;
		if(xwaitAllFlag[Fpid] == 1){
			xwaitAllFlag[Fpid] = 0;
			resume(Fpid);
		}
	}

	//----------------------Lab4 changes------------------------------------------------------------
	freestk(pptr->pbase, pptr->plen);

	pstate = pptr->pstate;
	pptr->pstate = PRFREE;	

	if (pstate == PRCURR)
	  resched();
        else	
         if (pstate == PRWAIT)
         {
            semaph[pptr->psem].semcnt++;
	    dequeue(pid);
         } /* PRWAIT */
         else
         if (pstate == PRREADY)
	    dequeue(pid);
         else
         if (pstate == PRSLEEP)
         {
           if ( q[pid].qnext < NPROC ) /* qnext is not tail */
                q[q[pid].qnext].qkey += q[pid].qkey;  /* Account for the 
                                                              pid delay */
                dequeue(pid);
               /* The sleep queue may now be empty, or have a new first */
            	if ( slnempty = nonempty(clockq) ) 
                         sltop = &firstkey(clockq);

         } /*  PRSLEEP */
         else; /* default: PRSUSP, PRRECV  */
	
	restore(ps);
	return(OK);

} /* kill */
