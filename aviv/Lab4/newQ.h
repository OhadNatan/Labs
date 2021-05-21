typedef struct qnode{
int secs; //time to be allocated
int next; //next in the list
int size; // size of allocation memory
char** mptr; //pointer to the memory
}qnode;

extern int timer;

extern int headNewQ;  //pointer to the head of the list 
extern int counterInQ; // # of memory blocks that now temporery allocated
extern qnode newQ[50]; //list of temporery allocated 
extern int newQfree[50]; //indicator to know if the index is allocated
