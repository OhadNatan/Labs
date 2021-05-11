#include <conf.h>
#include <kernel.h>
#include <sleep.h>
#include <io.h>
#include <q.h>
#include <proc.h>

xmain()
{
    int pid;
    pid = xforksonfirst();
    while(1){
        if(pid != 0)
            printf("Father is running\n");
        else
            printf("Son is running\n");
        sleep(3);
    }
}
