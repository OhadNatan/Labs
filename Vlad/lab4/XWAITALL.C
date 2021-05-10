#include <conf.h>
#include <kernel.h>
#include <proc.h>
#include <q.h>
#include <sem.h>
#include "SONG.H"

//return ok - if father wait for son
//return -1 - if father have no sons
SYSCALL xwaitall() {
	int	ps, firstson, father; //get the first son from father
	struct	sons* pson;
	disable(ps);
	father = currpid;
	firstson = firstsonpid(father);

	if (isbadpid(firstson)) {  //check the sons pid
		restore(ps);
		return(SYSERR);
	}
	pson = &procson[father];
	if (pson->quantity == 0) { //check if father have sons
		restore(ps);
		return(-1);
	}
	
	restore(ps);
	while (pson->quantity > 0) {
		xwait();
		//while (proctab[firstson].pstate != PRFREE) { //if sons procces still run
		//	disable(ps);
		//	dequeue(firstson);           //remove sons procces from ready q
		//	enqueue(firstson, rdytail);  //puts sons procces last in ready q
		//	preempt = 0;                 //make sure father lives the CPU
		//	resched();
		//	restore(ps);
		//}
		//firstson = getfirstson(currpid);
	}
	return(OK);
}