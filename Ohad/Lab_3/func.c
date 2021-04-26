#include <butler.h>
#include "New_var.h"

int get_new_prio(int pid){
    int new_prio;

    if(pid < 4)
        return pprio[pid];

    if(runnable_time[pid]==0)
        return pprio[pid]+1;

    new_prio = 1 + (pprio[pid] * (runnable_time[pid] – current_time[pid]))/runnable_time[pid];

    if(new_prio < 1)
        return 1;

    if( new_prio > BTLRPRIO)
        return BTLRPRIO - 1;

    return new_prio;
}