#include <conf.h>
#include <kernel.h>
#include <proc.h>
#include <q.h>
#include <sem.h>
#include "SONG.H"

//return ok - if father wait for son
//return -1 - if father have no sons
SYSCALL xwait() {  
	int	ps, firstson,father; //get the first son from father
	struct	sons* pson;
	disable(ps);
	father = currpid;
	

	pson = &procson[father];
	if (pson->quantity == 0) { //check if father have sons
		restore(ps);
		return(-1);
	}
	firstson = firstsonpid(father);
	if (isbadpid(firstson)) {  //check the sons pid
		restore(ps);
		return(SYSERR);
	}
	
	firstson = getfirstson(currpid);
	restore(ps);
	while (proctab[firstson].pstate != PRFREE) { //if sons procces still run
		if (proctab[firstson].pstate == PRREADY) {
			disable(ps);
			//printf("i am here\n");
			dequeue(firstson);           //remove sons procces from ready q
			enqueue(firstson, rdytail);  //puts sons procces last in ready q
			preempt = 0;                 //make sure father lives the CPU
			//resched();                
			restore(ps);
		}
	}
	return(OK);
}