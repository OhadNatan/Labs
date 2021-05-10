#include <conf.h>
#include <kernel.h>
#include <proc.h>
#include <q.h>
#include "SONG.H"


int countAdd(int adder);

xmain()
{
    int sem;
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

    return 0;
}

int countAdd(int adder) {
    int i, count = 0;;
    for (i = 0;i < 10;i++)
        count = count + adder;
    return count;
}

