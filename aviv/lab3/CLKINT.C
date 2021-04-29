/* clkint.c - clkint */

#include <conf.h>
#include <kernel.h>
#include <sleep.h>
#include <io.h>
#include <dos.h>
#include <proc.h>
#include <sem.h>
#include <mem.h>
#include <q.h>
#include <mark.h>
#include <butler.h>
#include <bios.h>
#include <kbdio.h>

#include "lab3H.h"

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
        int tempHead; //temporery int that run on the ready Q  
        int max; // save the pid of the proc with the max priority in ready Q

	tod++;

        /*-----------------------------------------------------lab3-change--------------------------------------------------------------------------*/
        current_time[currpid]++;
        peffec[currpid] = calcPrio(currpid);
        tempHead = max = q[tempHead].qnext;

        //loop on the ready Q and inc the runnable time of each proc
        while(tempHead != -1)
        {
                if (q[tempHead].qkey>q[max].qkey) max = tempHead;
                runnable_time[tempHead]++;
                peffec[tempHead] = calcPrio(tempHead);
                tempHead = q[tempHead].qnext;
        }
        // q[rdytail].qprev= max; TODO
        /*-----------------------------------------------------lab3-change--------------------------------------------------------------------------*/
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

