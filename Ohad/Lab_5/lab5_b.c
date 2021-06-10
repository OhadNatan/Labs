#include <stdio.h>
#include <stdlib.h>
#include <dos.h>

#define ON (1)
#define OFF (0)

void ChangeSpeaker(int status);
void Sound(int hertz);
void NoSound(void);
void RTCupdate();//our new RTC
void RTCrestore();//to set the old RTC
void interrupt int_0x70_handler();//for new interrupt
void interrupt int9handler(); //for new interrupt
int sumInterval(); //sumrize the intervals
void PrintArrToScreen();//print the array
int compareForQsort(const void *a, const void *b);//compare func for the sort

volatile int old_0A1h_mask, old_70h_A_mask, x71h1, x71h2, x71h3;
volatile int counter, indexOnArray, prevCode, StartFlag;
int flag; 
int arrayOfIntervals[200] = {0};

void interrupt (*int9save)(void);
void interrupt (*int_70h_old)(void);

void main()
{
    counter = 0;
    indexOnArray = 0;
    prevCode = 0;
    StartFlag = 0;
    flag = 1;
    RTCupdate(); //update for the new RTC by the requests
    int_70h_old = getvect(0x70); //save the old interrupt
    setvect(0x70, int_0x70_handler); //set our interrupt
    printf("Please enter string ,no more thacn 200 chars(Stop by enter X)\n");
    int9save = getvect(9); //save the old interupt
    setvect(9, int9handler);
    while (flag != 0) //get input without print it
    {
        asm {
            MOV AH,0
            INT 16h
        }
    }
    setvect(0x70, int_70h_old); //set old interrupt
    setvect(9, int9save); ////set old interrupt
    RTCrestore(); //return to the old RTC 
    printf("timed:\n");
    PrintArrToScreen();
    printf("\nSorted:\n");
    qsort(arrayOfIntervals, indexOnArray, sizeof(int), compareForQsort);
    PrintArrToScreen();
    printf("\nmax_time = %d / %d secs \n min time = %d / %d secs\nmed time = %d \n", arrayOfIntervals[indexOnArray - 1], 1069, arrayOfIntervals[0], 1069, arrayOfIntervals[(indexOnArray - 1) / 2]);
    printf("total time = %d / 1069", sumInterval());
    NoSound();
}
//new 70h interrupt
void interrupt int_0x70_handler(void)
{
    int_70h_old();
    counter++;
}

void RTCupdate()
{
    asm {
    CLI
    PUSH AX
    IN AL,0A1h
    MOV BYTE PTR old_0A1h_mask,AL
    AND AL,0FEh
    OUT 0A1h,AL

    IN AL,70h
    MOV AL,0Ah
    OUT 70h,AL
    MOV AL,8Ah
    OUT 70h,AL
    IN AL,71h
    MOV BYTE PTR x71h1,AL
    MOV BYTE PTR old_70h_A_mask,AL
    AND AL,10000000b
    OR AL,00110011b   
    OUT 71h,AL
    IN AL,71h
    IN AL,70h
    MOV AL,0Bh
    OUT 70h,AL
    MOV AL,8Bh
    OUT 70h,AL
    IN AL,71h
    MOV BYTE PTR x71h2,AL
    OR AL,40h
    OUT 71h,AL
    IN AL,71h
    MOV byte ptr x71h3,AL
    IN AL,021h
    AND AL,0FBh
    OUT 021h,AL

    IN AL,70h
    MOV AL,0Ch
    OUT 70h,AL
    IN AL,70h
    MOV AL,8Ch
    OUT 70h,AL
    IN AL,71h
    IN AL,70h
    MOV AL,0Dh
    OUT 70h,AL
    IN AL,70h
    MOV AL,8Dh
    OUT 70h,AL
    IN AL,71h
    
    POP AX
    STI
    }
}

void RTCrestore()
{
    asm {
         CLI
         PUSH AX
         MOV AL, BYTE PTR old_0A1h_mask
         OUT 0A1h,AL
         IN AL,70h 
         MOV AL,0Ah
         OUT 70h,AL
         MOV AL,8Ah
         OUT 70h,AL
         IN AL,71h
         MOV AL, BYTE PTR old_70h_A_mask
         OUT 71h,AL
         IN AL,71h
         POP AX
         STI

    } // asm
}

void ChangeSpeaker(int status)
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

    if (status == ON)
        portval |= 0x03;
    else portval &= ~0x03;
    // outportb( 0x61, portval );
    asm {
        PUSH AX
        MOV AX,portval
        OUT 61h,AL
        POP AX
    } // asm

} /*--ChangeSpeaker( )----------*/

void Sound(int hertz)
{
    unsigned divisor = 1193180L / hertz;

    ChangeSpeaker(ON);

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
        MOV AX, divisor
        AND AX, 0FFh
        OUT 42h, AL
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

void NoSound(void)
{
    ChangeSpeaker(OFF);
} /*--NoSound( )------*/

void interrupt int9handler(void)
{
    int scan_code, is_pushed;
    asm {
        PUSH AX
        MOV AH, 0
        IN   AL,60h   
        mov scan_code, AX
        POP AX
    }

    is_pushed = scan_code & 0x80;

    if (indexOnArray == 200) //check if the array is full
        flag = 0;

    if (is_pushed == 0)
    {
        if (scan_code == 45)
            flag = 0;

        if (prevCode == 0) //check if this is not the same char
        {
            indexOnArray = 0;
            counter = 0;
            prevCode = scan_code;
            Sound(1);
        }
        else if (scan_code != prevCode) // check if this is other char
        {
            Sound(arrayOfIntervals[indexOnArray - 1]);
            arrayOfIntervals[indexOnArray++] = counter;
            counter = 0;
            prevCode = scan_code;
        }
    }
    int9save();
}

int compareForQsort(const void *a, const void *b)
{
    int *x = (int *)a;
    int *y = (int *)b;
    return *x - *y;
}

void PrintArrToScreen()
{
    int i;
    for (i = 0; i < indexOnArray; i++)
        if (arrayOfIntervals[i] != 0)
            printf("%d ", arrayOfIntervals[i]);
}
int sumInterval()
{
    int i;
    long int sum = 0;
    for (i = 0; i < indexOnArray; i++)
        if (arrayOfIntervals[i] != 0)
            sum += arrayOfIntervals[i];
    return sum;
}