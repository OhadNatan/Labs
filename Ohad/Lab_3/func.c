#include <conf.h>
#include <kernel.h>
#include <proc.h>
#include <mem.h>
#include <butler.h>
#include <q.h>
#include "New_var.h"

int get_new_prio(int pid){
    int new_prio;

    if(pid < 4)
        return pprio[pid];

    if(runnable_time[pid]==0)
        return pprio[pid]+1;

    new_prio = 1 + (pprio[pid] * (runnable_time[pid] - current_time[pid])) / runnable_time[pid];
    
    if(new_prio < 1)
        return 1;

    if( new_prio > BTLRPRIO)
        return BTLRPRIO - 1;

    return new_prio;
}

int max_proc_prio(){
    int pid_to_check;
    int max_prio = 0;

    pid_to_check = q[rdyhead].qnext;

    while (pid_to_check != -1)
    {
            if(peffec[pid_to_check] > max_prio)
                max_prio = peffec[pid_to_check];
            pid_to_check = q[pid_to_check].qnext;
    }/* finding the max prio at ready queue*/

    return max_prio;
}

int max_proc_prio_pid(){
    int i;
    int max_prio = max_proc_prio();
    for (i=0 ; i<NPROC ; i++ ){
        if(peffec[i] == max_prio)
            return i;
    }
    return 0;
}/*Return the pid of the proc with the max prio*/