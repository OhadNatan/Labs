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
                runnable_time[pid_to_check] += 1;
                pid_to_check = q[pid_to_check].qnext;
        }/* runnable_time update*/

        current_time[currpid] += 1; /*current_time update*/

        peffec[currpid] = get_new_prio(currpid);

        // pptr = &proctab[currpid];
        // pptr->pprio = get_new_prio(currpid);

        pid_to_check = q[rdyhead].qnext;

        while (pid_to_check != -1)
        {
                peffec[pid_to_check] = get_new_prio(pid_to_check);
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

