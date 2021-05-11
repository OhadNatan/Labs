#include <conf.h>
#include <kernel.h>
#include <sleep.h>
#include <io.h>
#include <q.h>
#include <proc.h>
#include "FSONS.h"

xmain()
{
    int pid,pid2;
    int cnt1=0,cnt2=0;
    int oldpid = getpid();
    pid = xfork();
    if(pid!=0)
        pid2 = xfork();
    xwaitAll();
    while(1){
        if(pid == 0){
            cnt1++;
            printf("Son\n");
            if(cnt1==2)
                kill(getpid());
        }
        else if(pid2 == 0){
            cnt2++;
            printf("Son2\n");
            if(cnt2==6)
                kill(getpid());
        }
        else
            printf("Father\n");
        sleep(2);
    }
}
