#include <conf.h>
#include <kernel.h>
#include <proc.h>
#include <q.h>
#include <sleep.h>
#include "SONG.H"




xmain()
{
    int pid, pid2;
    int cnt1 = 0, cnt2 = 0;
    int oldpid = getpid();
    pid = xfork();
    if (pid != 0)
        pid2 = xfork();
    xwaitall();
    while (1) {
        if (pid == 0) {
            cnt1++;
            printf("Son the cnt1 : %d\n",cnt1);
            if (cnt1 == 2)
                kill(getpid());
        }
        else if (pid2 == 0) {
            cnt2++;
            printf("Son2 the cnt2 : %d\n", cnt2);
            if (cnt2 == 6)
                kill(getpid());
        }
        else
            printf("Father\n");
        sleep(2);
    }
    /*int sem;
    sem = screate(0);
    if (xfork() != 0) {
        if (xfork() != 0) {
            if (xfork() != 0) {
                printf("xmain() - father\n");
                printf("xmain() - father : the quantity is %d\n", procson[currpid].quantity);
                xwaitall();
                printf("xmain() - father : after xwaitall()\n");
                printf("xmain() - father : the quantity is %d\n", procson[currpid].quantity);
                printf("xmain() - father : the end for father\n");
            }
            else {
                wait(sem);
                printf("xmain() - third son \n");
                return 0;
            }
        }else{
            wait(sem);
            printf("xmain() - second son \n");
            signal(sem);
            return 0;
        }
    }
    else {
        printf("xmain() - first son \n");
        signal(sem);
        return 0;
    }

    return 0;*/
}