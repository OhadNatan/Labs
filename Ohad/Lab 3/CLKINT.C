/* clkint.c - clkint */

#include <conf.h>
#include <kernel.h>
#include <sleep.h>
#include <io.h>
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
        int pid;
        
	tod++;

        pid=getpid();
        pptr = &proctab[pid];
        if (pptr->pprio != 100 && pptr->pprio != 0){
                
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

