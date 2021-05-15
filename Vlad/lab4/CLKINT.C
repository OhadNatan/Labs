/* clkint.c - clkint */

#include <conf.h>
#include <kernel.h>
#include <sleep.h>
#include <io.h>
//Edit Vlad
#include "GETMEMFT.H"
//

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
    //Edit Vlad
    struct memfortime* mtime;
    int freeslot;
    //

    tod++;
    //Edit Vlad
    for (i = 0;i < quantitymemtime;i++) {
        if ((freeslot = getnextused(i)) == -1) {
            break;
        }
        mtime = &memtime[freeslot];
        if (mtime->stoptime == tod) {
            printf("i am here\n");
            freemem(mtime->ptr, mtime->nbyte);
            *mtime->ptr = NULL;
            mtime->ptr = NULL;
            
            i = freeslot;
            freeused[freeslot] = 0;
        }
    }
    //
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

