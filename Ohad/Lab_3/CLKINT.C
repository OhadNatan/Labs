/* clkint.c - clkint */

#include <conf.h>
#include <kernel.h>
#include <sleep.h>
#include <io.h>
#include <q.h>
#include <proc.h>
//**Lab3
#include "New_var.H"

/*------------------------------------------------------------------------
 *  clkint  --  clock service routine
 *  called at every clock tick and when starting the deferred clock
 *------------------------------------------------------------------------
 */
INTPROC clkint(mdevno)
int mdevno;				/* minor device number		*/
{
	int	i;
        int resched_flag;
        int pid_to_peffec;
        int tempPrio;
        struct	pentry	*pptr;		/* pointer to proc. table entry */

	tod++;

        pid_to_peffec = q[rdyhead].qnext;
        while (pid_to_peffec != -1)
        {
                if (pid_to_peffec > 3 && pid_to_peffec < 30)
                        runnable_time[pid_to_peffec]++;
                pid_to_peffec = q[pid_to_peffec].qnext;
        }/* runnable_time update*/

        current_time[getpid()]++; /*current_time update*/

        if (getpid() > 3 && runnable_time[getpid()]!=0){
                pptr = &proctab[getpid()];
                tempPrio = 1 + (((peffec[getpid()] * (runnable_time[getpid()]) - current_time[getpid()])) / runnable_time[getpid()]);
                 if (tempPrio>=1)
                        {
                                pptr->pprio= tempPrio;
                        }
                        else
                        {
                                pptr->pprio=1;
                        }
        }

        pid_to_peffec = q[rdyhead].qnext;

        while (pid_to_peffec != -1)
        {
                if (pid_to_peffec > 3 && pid_to_peffec <= 100){
                        pptr = &proctab[pid_to_peffec];
                        tempPrio = 1 + (peffec[pid_to_peffec] * (runnable_time[pid_to_peffec] - current_time[pid_to_peffec]) / runnable_time[pid_to_peffec]);
                        if (tempPrio>=1)
                        {
                                pptr->pprio= tempPrio;
                                q[pid_to_peffec].qkey = tempPrio;
                        }
                        else
                        {
                                pptr->pprio=1;
                                q[pid_to_peffec].qkey = 1;
                        }

                }
                pid_to_peffec = q[pid_to_peffec].qnext;
        }/* pprio update*/


        resched_flag = 0;
	if (slnempty)
		if ( (--*sltop) <= 0 )
                     {
                        resched_flag = 1;
			wakeup();
                     } /* if */
	if ( (--preempt) <= 0 )
             resched_flag = 1;

       if (resched_flag == 1)
 		resched();

}

