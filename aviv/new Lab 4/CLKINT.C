/* clkint.c - clkint */

#include <conf.h>
#include <kernel.h>
#include <sleep.h>
#include <io.h>
#include "newQ.h"
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

        
	tod++;
        
        resched_flag = 0;
          /*------------------------change lab 4 task4 --------------------------------------------------*/
        counter++;
        if (counterOfProc>0){
                if (sleepQueue[sleepHead].secs<=0)
                        deleteFromQ();
                if(timer%18==0)
                        sleepQueue[sleepHead].secs--;
        }
          /*------------------------change lab 4 task 4--------------------------------------------------*/
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

