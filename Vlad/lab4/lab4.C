#include <conf.h>
#include <kernel.h>
#include <proc.h>
#include <q.h>
#include <sleep.h>
#include "SONG.H"
#include "GETMEMFT.H"


xmain()
{
    int nbytes = 10;
    int secs = 5;
    void **ptr = NULL;
    int check;
    printf("xmain : before the getmemfortime()\n");
    check = getmemfortime(nbytes, secs, &ptr);
    if ( check == NULL)
        printf("unsuccessful \n");
    if(check == -1)
        printf("unsuccessful \n");
    else
        printf("successful\n");

    printf("xmain : after the getmemfortime()\n");
    if (ptr == NULL) 
        printf("xmain - before sleep : ptr is NULL \n");
    else
        printf("xmain - before sleep : ptr is NOT! NULL \n");
    sleep(6);
    if (ptr == NULL)
        printf("xmain - after sleep : ptr is NULL \n");
    else
        printf("xmain - after sleep : ptr is NOT! NULL \n");
    return 0;
    
}


