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
	word byte_number;
	struct	mblock	*p, *q, *leftover,*newP;

	disable(ps);
	if ( nbytes==0 ) {
		restore(ps);
		return( NULL );
	}
	nbytes = roundew(nbytes);
	//changes for lab 4------------------------------------------------------------------------------
	byte_number= nbytes + 4;
	for ( q=&memlist, p=q->mnext ; (char *)p != NULL ; q=p, p=p->mnext )
		if ( p->mlen == nbytes+8) {        //padding with block size 4 in every side
			newP=(struct mblock *)((char *)p+4);
			p->mlen = 4;
			((struct mblock *)((char *)p+4+nbytes))->mnext = p->mnext;
			((struct mblock *)((char *)p+4+nbytes))->mlen = 4;
			p->mnext = ((struct mblock *)((char *)p+4+nbytes));
			restore(ps);
			return( (char *) newP );
		} else if ( p->mlen > nbytes+8 ) {           //padding with one block size 4
			leftover = (struct mblock *)( (char *)p + nbytes + 4);
			newP=(struct mblock *)((char *)p+4);
			leftover->mlen = p->mlen - nbytes - 4;
			leftover->mnext = p->mnext;
			p->mlen = 4;
			p->mnext = leftover;
			restore(ps);
			return( (char *) newP );
	//  changes for lab 4------------------------------------------------------------------------------
		}
	restore(ps);
	return( NULL );
}
