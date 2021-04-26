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
        int pid_to_check;
        int new_prio;
        struct	pentry	*pptr;		/* pointer to proc. table entry */

	tod++;

        pid_to_check = q[rdyhead].qnext;
        while (pid_to_check != -1)
        {
                // printf("\nchecking %d pid int ready queue\n",pid_to_check);
                runnable_time[pid_to_check]++;
                pid_to_check = q[pid_to_check].qnext;
        }/* runnable_time update*/



        if (currpid > 3 && runnable_time[currpid]!=0){
                pptr = &proctab[currpid];
                new_prio = 1 + peffec[currpid] * ((runnable_time[currpid] - current_time[currpid]) / runnable_time[currpid]);
                if (new_prio > 1 && new_prio < 100 )
                                pptr->pprio = new_prio;
                else
                                pptr->pprio = 1;
        }

        current_time[currpid]++; /*current_time update*/

        pid_to_check = q[rdyhead].qnext;

        while (pid_to_check != -1)
        {
                if (pid_to_check > 3){
                        pptr = &proctab[pid_to_check];
                        new_prio = 1 + peffec[pid_to_check] * ((runnable_time[pid_to_check] - current_time[pid_to_check]) / runnable_time[pid_to_check]);
                        if (new_prio > 1 && new_prio < 100 )
                                pptr->pprio = new_prio;
                        else
                                pptr->pprio = 1;
                }
                pid_to_check = q[pid_to_check].qnext;
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

