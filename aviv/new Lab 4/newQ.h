typedef struct qnode{
int secs; //time to be allocated
int next; //next in the list
int size; // size of allocation memory
char** mptr; //pointer to the memory
}qnode;

extern int counter;

extern int sleepHead;  //pointer to the head of the list 
extern int counterOfProc; // # of memory blocks that now temporery allocated
extern qnode sleepQueue[50]; //list of temporery allocated 
extern int newQfree[50]; //indicator to know if the index is allocated
//extern char* memalloction[50]; //poiters to memory blocks