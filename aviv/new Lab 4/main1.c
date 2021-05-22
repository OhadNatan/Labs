#include <conf.h>
#include <kernel.h>
#include <sleep.h>
#include <io.h>
#include <q.h>
#include <proc.h>
#include "FSONS.h"

xmain()
{
    //task 1 

    // int pidToCheck;
    // pidToCheck = xforksonfirst();
    // printf("pid is %d\n",pidToCheck);
    // if (pidToCheck != 0 ){
    //     printf("father\n");
    // }
    // else 
    //     printf("son\n");
    // return;
    


    // task 2 

    // int pid;
   
    // pid = xfork();
    // 
    // if(pid!=0){
    //    xwait(); 
    //    printf("im the father\n");
    // }
    // else {
    //     printf(" im the son and if im first than all good\n");
    //     kill(getpid());
    // }
    // return;




    //task 3

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
    // return;
    // }
    


    //task4

    // int *iptr;
    // int *iptr2;

    // getmemForTime(2 * sizeof(int), 6, &iptr);
    // getmemForTime(2 * sizeof(int), 3, &iptr2);

    // if (iptr != NULL)
    // {
    //     iptr[0] = 1;
    //     iptr[1] = 2;
    // }

    // if (iptr2 != NULL)
    // {
    //     iptr2[0] = 3;
    //     iptr2[1] = 4;
    // }

    // printf("\nind 0 - %d\nind 1 - %d\n", iptr[0], iptr[1]);
    // printf("\nind2 0 - %d\nind2 1 - %d\n", iptr2[0], iptr2[1]);

    // sleep(5);

    // if (iptr2 == NULL)
    //     printf("\nGood\n");
    // else
    //     printf("\nError\n");

    // printf("\nind2 0 - %d\nind2 1 - %d\n", iptr2[0], iptr2[1]);

    // sleep(5);

    // if (iptr  == NULL)
    //     printf("\nGood2\n");
    // else
    //     printf("\nError 2\n");

    // printf("\nind 0 - %d\nind 1 - %d\n", iptr[0], iptr[1]);

    // return;
  


    //task 5

    // int * x;
    // int * tempMem = (int *) getmem(sizeof(int));
    // *tempMem = 10;
    // kprintf("\npblock original - %d\n",tempMem);
    // printf("X = %d\n", *tempMem );
    // x = (int *) getmem(sizeof(int));
    // kprintf("\npblock2 original - %d\n",x);
    // *x = 5;
    // freep((char*)tempMem);
    // printf("X = %d\n", *tempMem );
    // printf("X2 = %d\n", *x );

    // tempMem = (int *) getmem(sizeof(int));
    // kprintf("\npblock3 original - %d\n",tempMem);
    // return ;
}
