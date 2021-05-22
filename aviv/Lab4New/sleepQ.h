
typedef struct qnode{
int secs; //time
int next; //index of the next node
int size; // number of bytes to allocate
char** mptr; //pointer to the memory block
}qnode;

extern qnode sleepQueue[50]; //queue of memory to free by time

extern int counter;
extern int sleepHead;  //pointer to the head of the queue 
extern int counterOfProc; // counter of memory blocks that allocated by for temporery time
extern int newQfree[50]; //indicator for free index