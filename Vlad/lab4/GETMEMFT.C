#include <conf.h>
#include <kernel.h>
#include <mem.h>
#include <sleep.h>
#include "GETMEMFT.H"





SYSCALL getmemfortime(int nbytes, int secs, void** ptr){
	int ps;
	int insertresult;
	disable(ps);
	if (quantitymemtime < MAXMEMTIME) {
		if ((*ptr = getmem(nbytes)) == NULL) {
			restore(ps);
			return(NULL);
		}
		insertresult = inserttimemem(roundew(nbytes), secs, ptr);
		restore(ps);
		return insertresult;
	}
	else {
		restore(ps);
		return -1;
	}
}

int inserttimemem(int nbytes, int secs, void** ptr) {
	int freeslot;
	struct memfortime* mtime;

	if (quantitymemtime < MAXMEMTIME) {
		if ((freeslot = getnextfree(0)) == -1) {
			return -1;
		}
		else {
			mtime = &memtime[freeslot];
			mtime->nbyte = nbytes;
			mtime->ptr = ptr;
			mtime->stoptime = secs * CLOKTICKSINSEC + tod;
			quantitymemtime++;
			freeused[freeslot] = 1;
			return 1;
		}
	}
	else return -1;
}

int getnextfree(int number) {
	int i;
	for (i = number;i < MAXMEMTIME;i++) {
		if (freeused[i] == 0)
			return i;
	}
	return -1;
}

int getnextused(int number) {
	int i;
	for (i = number;i < MAXMEMTIME;i++) {
		if (freeused[i] == 1)
			return i;
	}
	return -1;
}

