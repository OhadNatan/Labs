/* chprio.c - chprio */

#include <conf.h>
#include <kernel.h>
#include <proc.h>
#include "lab3H.H"

/*------------------------------------------------------------------------
 *  chprio  --  change the scheduling priority of a process
 *------------------------------------------------------------------------
 */
SYSCALL chprio(pid,newprio)
	int	pid;
	int	newprio;		/* newprio > 0			*/
{
	int	oldprio;
	struct	pentry	*pptr;
        int	ps;

	disable(ps);
	if (isbadpid(pid) || newprio<=0 ||
	    (pptr = &proctab[pid])->pstate == PRFREE) {
		restore(ps);
		return(SYSERR);
	}
	oldprio = pptr->pprio;
	pptr->pprio = newprio;
	pprio[pid] = newprio;
	restore(ps);
	return(oldprio);
}
