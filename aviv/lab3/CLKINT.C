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
        while (q[tempHead].qkey < MAXINT){
                runnable_time[tempHead]++;
                peffec[tempHead] = calcPrio(tempHead);
                q[tempHead].qkey = peffec[tempHead];
                tempHead = q[tempHead].qnext;
        }

        // if (tod%70 == 0 && currpid!=3){
        //         int i;
        //         for(i=0 ; i<30 ; i++){
        //                 if(runnable_time[i]!=0){
        //                         kprintf("runnable_time: pid %d value %d\n", i, runnable_time[i]);
        //                 }
        //                 if(current_time[i]!=0){
        //                         kprintf("current_time: pid %d value %d\n", i, current_time[i]);
        //                 }
        //         }
        // }

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

