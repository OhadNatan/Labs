/* clkint.c - clkint */

#include <conf.h>
#include <kernel.h>
#include <sleep.h>
#include <io.h>

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
    //------------------------------------Edit Vlad-----------------------------------------
    for (i = 4; i < NPROC; i++) {
        if (proctab[i]->pstate == PRREADY)
            proctab[i]->runnable_time++;
        if (proctab[i]->pstate == PRCURR)
            proctab[i]->current_time++;
        proctab[i]->peffec =
            1 + (proctab[i]->pprio * (proctab[i]->runnable_time - proctab[i]->current_time)) / proctab[i]->runnable_time;

    }
    //-----------------------------------End Edit Vlad--------------------------------------

    resched_flag = 0;
    if (slnempty)
        if ((-- * sltop) <= 0)
        {
            resched_flag = 1;
            wakeup();
        } /* if */
    if ((--preempt) <= 0)
        resched_flag = 1;

    if (resched_flag == 1)
        resched();

}

