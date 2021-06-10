#include <stdio.h>
#include <stdlib.h>
#include <dos.h>

#define ON (1)
#define OFF (0)

void ChangeSpeaker( int status );
void Sound( int hertz );
void NoSound( void );
void programPit(int divNum);
void revProgramPit();
void interrupt int8handler();
void interrupt int9handler ();
int sumInterval();
void PrintArrToScreen();
int compareForQsort(const void *a,const void *b);

volatile int counter, indexOnArray, prevCode, StartFlag;
int flag;
int arrayOfIntervals[200] = {0};

void interrupt (*int9save)(void);
void interrupt (*int8save)(void);


void main(){
    counter = 0;
    indexOnArray = 0;
    prevCode = 0;
    StartFlag = 0;
    flag = 1;
    programPit(700);
    int8save = getvect(8);
    setvect(8,int8handler);
    printf("Please enter string ,no more thacn 200 chars(Stop by enter X)\n");
    int9save = getvect(9);
    setvect(9,int9handler);
    while(flag != 0){
        asm{
            MOV AH,0
            INT 16h
        }
    }
    setvect(8,int8save);
    setvect(9,int9save);
    revProgramPit();
    printf("timed:\n");
    PrintArrToScreen();
    printf("\nSorted:\n");
    qsort(arrayOfIntervals, indexOnArray, sizeof(int), compareForQsort);
    PrintArrToScreen();
    printf("\nmax_time = %d / %d secs \n min time = %d / %d secs\nmed time = %d \n", arrayOfIntervals[indexOnArray-1], 1069, arrayOfIntervals[0], 1069 , arrayOfIntervals[(indexOnArray-1)/2]); 
    printf("total time = %d / 1069", sumInterval());
    NoSound();
}


void ChangeSpeaker( int status )
 {
  int portval;
//   portval = inportb( 0x61 );

      portval = 0;
   asm {
        PUSH AX
        IN AL,61h
        MOV byte ptr portval,AL
        POP AX
       }

    if ( status==ON )
     portval |= 0x03;
      else
       portval &=~ 0x03;
        // outportb( 0x61, portval );
        asm {
          PUSH AX
          MOV AX,portval
          OUT 61h,AL
          POP AX
        } // asm

	} /*--ChangeSpeaker( )----------*/


void Sound( int hertz )	{
        unsigned divisor = 1193180L / hertz;

        ChangeSpeaker( ON );

    //        outportb( 0x43, 0xB6 );
        asm {
            PUSH AX
            MOV AL,0B6h
            OUT 43h,AL
            POP AX
        } // asm


        //       outportb( 0x42, divisor & 0xFF ) ;
        asm {
            PUSH AX
            MOV AX,divisor
            AND AX,0FFh
            OUT 42h,AL
            POP AX
        } // asm


        //        outportb( 0x42, divisor >> 8 ) ;

        asm {
            PUSH AX
            MOV AX,divisor
            MOV AL,AH
            OUT 42h,AL
            POP AX
        } // asm
    } /*--Sound( )-----*/

    void NoSound( void ){
        ChangeSpeaker( OFF );
    } /*--NoSound( )------*/

void programPit(int divNum){
    asm{
        CLI
        push AX
        push BX
        MOV AL,36H 
        OUT 43H,AL
        MOV BX, divNum 
        MOV AL,BL        
        OUT 40H,AL
        MOV AL,BH     
        OUT 40H,AL
        pop BX  
        pop AX
        STI
    }
}

void revProgramPit(){
    asm{
        CLI
        MOV AL,36H 
        OUT 43H,AL
        MOV BX, 0 
        MOV AL,BL        
        OUT 40H,AL
        MOV AL,BH     
        OUT 40H,AL
        STI
    }
}

void interrupt int8handler(void)
{
   counter++;
   int8save();
}

void interrupt int9handler (void){ 
    int scan_code, is_pushed;
    asm{
        PUSH AX
        MOV AH, 0
        IN   AL,60h   
        mov scan_code, AX
        POP AX
    }

    is_pushed = scan_code & 0x80 ;

    if(indexOnArray == 200)
        flag = 0; 

    if(is_pushed == 0){
        if (scan_code == 45)
            flag = 0; 
        
        if(prevCode == 0){
            indexOnArray = 0;
            counter = 0;
            prevCode = scan_code;
            Sound(1);
        }
        else if(scan_code != prevCode){
            Sound(arrayOfIntervals[indexOnArray-1]);
            arrayOfIntervals[indexOnArray++] = counter;
            counter = 0;
            prevCode = scan_code;
        }
    }
    int9save();
}

int compareForQsort(const void *a,const void *b) {
    int *x = (int *) a;
    int *y = (int *) b;
    return *x - *y;
}

void PrintArrToScreen(){
    int i;
    for(i = 0; i < indexOnArray ; i++)
        if(arrayOfIntervals[i] != 0)    
            printf("%d ", arrayOfIntervals[i]);
}
int sumInterval(){
    int i;
    long int sum = 0;
    for(i = 0; i < indexOnArray ; i++)
        if(arrayOfIntervals[i] != 0)    
            sum += arrayOfIntervals[i];
    return sum;
}