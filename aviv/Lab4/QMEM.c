extern qmemEntry memQ[50];

insert(char* ptr, int secs){
    int i;
    for(i=0; i<50 && (memQ[i].next != 0); i++ ){
        
    }
}


INTPROC insertd(pid, head, key)
	int	pid;
	int	head;
	int	key;
{
	int	next;			/* runs through list		*/
	int	prev;			/* follows next through list	*/

	for(prev=head,next=q[head].qnext ;
	    q[next].qkey < key ; prev=next,next=q[next].qnext)
		key -= q[next].qkey;
	q[pid].qnext = next;
	q[pid].qprev = prev;
	q[pid].qkey  = key;
	q[prev].qnext = pid;
	q[next].qprev = pid;
	if (next < NPROC)
		q[next].qkey -= key;
	return(OK);
}