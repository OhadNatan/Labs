/* clkint.c - clkint */

#include <conf.h>
#include <kernel.h>
#include <sleep.h>
#include <io.h>
#include <q.h>
#include <proc.h>
#include "lab3H.h"
/*------------------------------------------------------------------------
 *  clkint  --  clock service routine
 *  called at every clock tick and when starting the deferred clock
 *------------------------------------------------------------------------
 */
INTPROC clkint(mdevno)
int mdevno;				/* minor device number		*/
{
	int	i,tempHead;
        int resched_flag;

      
	tod++;
        current_time[currpid]++;
        peffec[currpid] = calcPrio(currpid);
        tempHead = q[rdyhead].qnext;

        //loop on the ready Q and inc the runnable time of each proc
        while(q[tempHead].qnext != -1)
        {
                runnable_time[tempHead]++;
                peffec[tempHead] = calcPrio(tempHead);
                q[tempHead].qkey = peffec[tempHead];
                tempHead = q[tempHead].qnext;
        }
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

