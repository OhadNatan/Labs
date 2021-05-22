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
	int i;
	int  FatherPid;  //Lab4 Change--

	disable(ps);
	if (isbadpid(pid) || (pptr = &proctab[pid])->pstate==PRFREE) {
		restore(ps);
		return(SYSERR);
	}
	if (--numproc == 0)
		xdone();


	//Lab4 change
	
	FatherPid = pidOfFather[pid];
	numOfSons[FatherPid]--; //update the number of the sons of this process
	if(son_first[FatherPid] == pid){ //check if the pid is first son of process
		if(XWAITflag[FatherPid] == 1){ //update the flags of the syscall
			XWAITflag[FatherPid] = 0;
			resume(FatherPid); //wake up the father
		}
	}
	if(numOfSons[FatherPid] == 0){  //update that there is no first son if there are not sons at all
		son_first[FatherPid] = 0;
		if(XWaitAllFlag[FatherPid] == 1){ //update the flags of the syscall
			XWaitAllFlag[FatherPid] = 0;
			resume(FatherPid); //wake up the father
		}
	}

	//Lab4 change


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
