#include <conf.h>
#include <kernel.h>
#include <proc.h>
#include <q.h>
#include <sem.h>
#include "SONG.H"

//return ok - if father wait for son
//return -1 - if father have no sons
SYSCALL xwaitall() {
	int	ps, firstson, father, quantity; //get the first son from father
	struct	sons* pson;
	disable(ps);
	father = currpid;
	

	pson = &procson[father];
	quantity = pson->quantity;
	if (pson->quantity == 0) { //check if father have sons
		restore(ps);
		return(-1);
	}
	firstson = firstsonpid(father);
	if (isbadpid(firstson)) {  //check the sons pid
		restore(ps);
		return(SYSERR);
	}
	
	
	restore(ps);
	while (quantity > 0) {
		
		xwait();
		quantity = pson->quantity;
	}
	return(OK);
}