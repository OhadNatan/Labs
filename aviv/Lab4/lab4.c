#include <conf.h>
#include <kernel.h>
#include <sleep.h>
#include <io.h>
#include <q.h>
#include <proc.h>
#include "FSONS.h"

xmain()
{
    // int pid,pid2;
    // int cnt1=0,cnt2=0;
    // int oldpid = getpid();
    // pid = xfork();
    // if(pid!=0)
    //     pid2 = xfork();
    // xwaitAll();
    // while(1){
    //     if(pid == 0){
    //         cnt1++;
    //         printf("Son\n");
    //         if(cnt1==2)
    //             kill(getpid());
    //     }
    //     else if(pid2 == 0){
    //         cnt2++;
    //         printf("Son2\n");
    //         if(cnt2==6)
    //             kill(getpid());
    //     }
    //     else
    //         printf("Father\n");
    //     sleep(2);
    // }

    int *iptr;
    int *iptr2;

    getmemForTime(2 * sizeof(int), 6, &iptr);
    getmemForTime(2 * sizeof(int), 10, &iptr2);

    if (iptr != NULL)
    {
        iptr[0] = 1;
        iptr[1] = 2;
    }

    if (iptr2 != NULL)
    {
        iptr2[0] = 3;
        iptr2[1] = 4;
    }

    printf("\nind 0 - %d\nind 1 - %d\n", iptr[0], iptr[1]);
    printf("\nind2 0 - %d\nind2 1 - %d\n", iptr2[0], iptr2[1]);

    sleep(6);

    if (iptr == NULL)
        printf("\nGood\n");
    else
        printf("\nError\n");

    printf("\nind 0 - %d\nind 1 - %d\n", iptr[0], iptr[1]);

    sleep(4);

    if (iptr2 == NULL)
        printf("\nGood2\n");
    else
        printf("\nError 2\n");

    printf("\nind2 0 - %d\nind2 1 - %d\n", iptr2[0], iptr2[1]);

    return;
}
