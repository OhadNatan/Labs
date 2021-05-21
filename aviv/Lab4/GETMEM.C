/* getmem.c - getmem */

#include <conf.h>
#include <kernel.h>
#include <mem.h>

/*------------------------------------------------------------------------
 *  getmem  --  allocate heap storage, returning lowest integer address
 *------------------------------------------------------------------------
 */
char *getmem(nbytes)
word nbytes;
{
	int	ps;
	word newNB;
	struct	mblock	*p, *q, *leftover,*n; //add *n for lab 4

	disable(ps);
	if ( nbytes==0 ) {
		restore(ps);
		return( NULL );
	}
	nbytes = roundew(nbytes);
	//  -----------------------------------lab 4 task 5---------------------------------------------
	newNB= nbytes + 4;
	for ( q=&memlist, p=q->mnext ;
		(char *)p != NULL ;
		q=p, p=p->mnext )
		if ( p->mlen == nbytes+8) { //pedding with 2 * block at size 4
			n=(struct mblock *)((char *)p+4);
			p->mlen = 4;
			((struct mblock *)((char *)p+4+nbytes))->mnext = p->mnext;
			((struct mblock *)((char *)p+4+nbytes))->mlen = 4;
			p->mnext = ((struct mblock *)((char *)p+4+nbytes));
			restore(ps);
			return( (char *) n );
		} else if ( p->mlen > nbytes+8 ) { //pedding with one block at size 4
			leftover = (struct mblock *)( (char *)p + nbytes + 4);
			n=(struct mblock *)((char *)p+4);
			leftover->mlen = p->mlen - nbytes - 4;
			leftover->mnext = p->mnext;
			p->mlen = 4;
			p->mnext = leftover;
			restore(ps);
			return( (char *) n );
	//  -----------------------------------lab 4 task 5---------------------------------------------
		}
	restore(ps);
	return( NULL );
}
