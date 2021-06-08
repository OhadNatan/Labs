#include <dos.h>
#include <stdio.h>
#include <stdlib.h>
#include <math.h>

#define MAXCHAR 200   //max char that can be input
#define STOP_BUTTON_ACKII 13   //the stop button to stop before MAXCHAR input (can be changed)
#define PUSH_BUTTON 1          //check if the button was pushed
#define RELEASE_BUTTON 2       //check if the button was released
#define FIRST_BUTTON 0         //check if the button was first in the sequenc
#define SECOND_BUTTON 1        //check if the button was second in the sequenc 
#define ON (1)                 //for ON the speaker
#define OFF (0)                //for off the speaker

volatile int persec70h = 1024; //how many ticks per second in TTC
volatile int persec8 = 1024; //how many ticks per second in INT8
volatile int latch = 1165; //the set latch is 700 , to sync with RTS latch = 1165
volatile int button = 0; //for know if the push was first button or second
volatile int click = -2; //for push and release
volatile int count8 = 0;  //count ticks for 8INT
volatile int count70h = 0;  //count ticks for INT70h

volatile int CurrentPlace = 0;//current plase in array CountTimeInterval
int CountTimeInterval8[MAXCHAR] = { 0 }; // save the difference between pushes with INT8
int CountTimeInterval70h[MAXCHAR] = { 0 }; // save the difference between pushes with INT70h
char currentChar ='a'; //current char that was pushed
char CharInInt9;       //the last char that was in INT 9

//for save old value of INT 70h
volatile char OldMaskA;
volatile char OldRegA1_70h;
volatile char OldRegA2_70h;
volatile char OldRegB1_70h;
volatile char OldRegB2_70h;
//

void interrupt newint8(void);  //new interrupt 8
void interrupt newint9(void);  //new interrupt 9
void interrupt newint70h(void);//new interrupt 70h

void selectionSort(int arr[], int n); //sort array
void swap(int* x, int* y);            //swap numbers
int MaxTime(int arr[], int n);        //max time in array
int MinTime(int arr[], int n);        //min time in array
int TotalTime(int arr[], int n);      //the sum of array
void Sound(int hertz);                //make sound by difference 
void ChangeSpeaker(int status);       //change status of speakers (ON,OFF)
void SetInt70h();                     //set the registers of int 70h
void RestoreInt70h();                 //restore the changes in int 70h
void SetNewLatch(int n);              //set new latch by the number n
void RestoreLatch();                  //restore to the old latch 
 
void interrupt (*int8save)(void);
void interrupt (*int9save)(void);
void interrupt (*int70hsave)(void);

int main() {
    int i,charcount = 0;
    char scan_char;
    
    //set the new interrupt
    int9save = getvect(9);
    setvect(9, newint9);

    SetNewLatch(latch);    //set new latch according the test (latch = 700)
    int8save = getvect(8);
    setvect(8, newint8);

    SetInt70h();
    int70hsave = getvect(0x70);
    setvect(0x70, newint70h);
    //

    printf("Please enter text up to %d chars -\n", MAXCHAR);
    printf("To finish prees enter\n");

    //do (get char from user) until (charcount == MACHAR) OR (the char is STOPCHAR)
    do {

        //printf("[main] before scanf\n");
        asm{
            MOV AH,0; /* BIOS read char from buffer option */
            INT 16h; /* BIOS read char from buffer        */

            MOV currentChar,AL; /* Transfer char to program          */
        }
            //printf("[main] char ackii - %d\n", currentChar);
        charcount++;
    } while ((int)currentChar != STOP_BUTTON_ACKII && charcount <= MAXCHAR);
    
    //restore the interrupt
    setvect(9, int9save);

    RestoreLatch();
    setvect(8, int8save);

    RestoreInt70h();
    setvect(0x70, int70hsave);

    ChangeSpeaker(OFF);
    //

    //--------------------------------------for int 8--------------------------------------
    printf("---------------------------------For INT8-------------------------------------- \n");
    printf("the time difference is- \n"); 
    for (i = 0;i < CurrentPlace;i++) {
        printf("[%d] ", CountTimeInterval8[i]);
        if (i % 8 == 0 && i != 0)
            printf("\n");
    }
    selectionSort(CountTimeInterval8, CurrentPlace);

    printf("\nthe time difference sorted- \n");
    for (i = 0;i < CurrentPlace;i++) {
        printf("[%d] ", CountTimeInterval8[i]);
        if (i % 8 == 0 && i != 0)
            printf("\n");
    }
    printf("\nMax Time = %d/%d sec, ", MaxTime(CountTimeInterval8, CurrentPlace), persec8);
    printf("Min Time = %d/%d sec\n", MinTime(CountTimeInterval8, CurrentPlace), persec8);
    printf("Med Time = %d/%d sec, ", CountTimeInterval8[(CurrentPlace - 1) / 2], persec8);
    printf("Total Time = %d/%d sec\n", TotalTime(CountTimeInterval8, CurrentPlace), persec8);
    //--------------------------------------for int 70h------------------------------------
    printf("--------------------------------For INT70h------------------------------------- \n");
    printf("the time difference is- \n");
    for (i = 0;i < CurrentPlace;i++) {
        printf("[%d] ", CountTimeInterval70h[i]);
        if (i % 12 == 0 && i != 0)
            printf("\n");
    }
    selectionSort(CountTimeInterval70h, CurrentPlace);

    printf("\nthe time difference sorted- \n");
    for (i = 0;i < CurrentPlace;i++) {
        printf("[%d] ", CountTimeInterval70h[i]);
        if (i % 12 == 0 && i != 0)
            printf("\n");
    }
    printf("\nMax Time = %d/%d sec, ", MaxTime(CountTimeInterval70h, CurrentPlace), persec70h);
    printf("Min Time = %d/%d sec\n", MinTime(CountTimeInterval70h, CurrentPlace), persec70h);
    printf("Med Time = %d/%d sec, ", CountTimeInterval70h[(CurrentPlace - 1) / 2], persec70h);
    printf("Total Time = %d/%d sec\n", TotalTime(CountTimeInterval70h, CurrentPlace), persec70h);

    printf("----------------------------INT8 comparison INT70h----------------------------- \n");
    printf("the difference between INT8 ticks and INT70h ticks\n");
    for (i = 0;i < CurrentPlace;i++) {
        printf("[%d] ", abs(CountTimeInterval70h[i]-CountTimeInterval8[i]));
        if (i % 12 == 0 && i != 0)
            printf("\n");
    }

    return 0;
}

void interrupt newint8(void) {

    count8++; //counter for num of ticks by int8
    int8save();

}

void interrupt newint70h(void) {
    //counter for num of ticks by int70h
    count70h++;
    int70hsave();

}

void interrupt newint9(void) {
    click++;               //increase the click by 1 for know if its push or relase
    if (click >= PUSH_BUTTON && (int)currentChar != STOP_BUTTON_ACKII) {
        //printf("[interrupt newint9] click = %d\n",click);
        if (click == PUSH_BUTTON) {   //check if call for INT was push or release
            //printf("[interrupt newint9] after if (click == 1)\n");
           
            if (button == FIRST_BUTTON) {   //check if the push was first button or not
                CharInInt9 = currentChar;   //save the button that was first push
                if (CurrentPlace == 0) {    //check if its the first push in the system 
                    ChangeSpeaker(ON);
                    Sound(1);               //for the first button the dif is 1
                }
                else {
                    if (CountTimeInterval8[CurrentPlace - 1] <= 0) //in case the difference is 0 (if the user clicked to fast)
                        Sound(1);
                    else
                        Sound(CountTimeInterval8[CurrentPlace - 1]);//set sound dif to be previous dif
                }
                count8 = 0;             //zero the count to start counting
                count70h = 0;             //zero the count to start counting
                button = SECOND_BUTTON; //change the button to know the next push is the second key
            }
            else {
                if (button == SECOND_BUTTON && CharInInt9 != currentChar) { //check if the push is second button and if its different button

                    CountTimeInterval8[CurrentPlace] = count8; //set the difference
                    CountTimeInterval70h[CurrentPlace] = count70h; //set the difference
                    CurrentPlace++;                          //move to next slot in count
                    button = FIRST_BUTTON;                   //change button to know the next push is first

                }
            }
        }
        else {
            //printf("[interrupt newint9] after else\n");
            if (click == RELEASE_BUTTON)//check if its push or release
                click = 0;              //if realase then the next is push
        }
    }
    int9save();
}

void selectionSort(int arr[], int n)
{
    int i, j, min_idx;

    // One by one move boundary of unsorted subarray
    for (i = 0; i < n - 1; i++)
    {
        // Find the minimum element in unsorted array
        min_idx = i;
        for (j = i + 1; j < n; j++)
            if (arr[j] < arr[min_idx])
                min_idx = j;

        // Swap the found minimum element with the first element
        swap(&arr[min_idx], &arr[i]);
    }
}

void swap(int* x, int* y)
{
    int temp = *x;
    *x = *y;
    *y = temp;
}

int MaxTime(int arr[], int n) {
    int Max = -1,i;
    for (i = 0;i < n;i++) 
        if (Max < arr[i])
            Max = arr[i];
    return Max;
}
int MinTime(int arr[], int n) {
    int  i;
    int Min = MaxTime(arr, n);
    for (i = 0;i < n;i++)
        if (Min > arr[i])
            Min = arr[i];
    return Min;
}
int TotalTime(int arr[], int n) {
    int Sum = 0;
    int i;
    for (i = 0;i < n;i++)
        Sum += arr[i];
    return Sum;
}

void Sound(int hertz){   //set the sount by hertz
    unsigned divisor = 1193180L / hertz;

    ChangeSpeaker(ON);

    //        outportb( 0x43, 0xB6 );
    asm{
      PUSH AX
      MOV AL,0B6h
      OUT 43h,AL
      POP AX
    } // asm


 //       outportb( 0x42, divisor & 0xFF ) ;
        asm{
          PUSH AX
          MOV AX,divisor
          AND AX,0FFh
          OUT 42h,AL
          POP AX
    } // asm


 //        outportb( 0x42, divisor >> 8 ) ;

        asm{
          PUSH AX
          MOV AX,divisor
          MOV AL,AH
          OUT 42h,AL
          POP AX
    } // asm

} /*--Sound( )-----*/

void ChangeSpeaker(int status){
    int portval;
    //   portval = inportb( 0x61 );

    portval = 0;
    asm{
         PUSH AX
         IN AL,61h
         MOV byte ptr portval,AL
         POP AX
    }

        if (status == ON)
            portval |= 0x03;
        else
            portval &= ~0x03;
    // outportb( 0x61, portval );
    asm{
      PUSH AX
      MOV AX,portval
      OUT 61h,AL
      POP AX
    } // asm

}

void SetInt70h() {

        asm{
  CLI         // Disable interrupts
  PUSH AX     // Interrupt may occur while updating

  IN AL,0A1h  // Make sure IRQ8 is not masked
  MOV OldMaskA,AL
  AND AL,0FEh // Set bit 0 of port 0A1 to zero
  OUT 0A1h,AL //

  IN AL,70h   // Set up "Write into status register A"
  MOV AL,0Ah  //
  OUT 70h,AL  //
  MOV AL,8Ah  //
  OUT 70h,AL  //
  IN AL,71h   //
  MOV BYTE PTR OldRegA1_70h,AL  // Save old value
  MOV OldRegA2_70h,AL
  AND AL,10100110b // Change only Rate
  OR AL,0110b // Make sure it is Rate =0110 (1Khz)
  OUT 71h,AL  // Write into status register A
  IN AL,71h   // Read to confirm write



  IN AL,70h  // Set up "Write into status register B"
  MOV AL,0Bh //
  OUT 70h,AL //
  MOV AL,8Bh //
  OUT 70h,AL //
  IN AL,71h  //
  MOV BYTE PTR OldRegB1_70h,AL // Save Old value
  AND AL,8Fh // Mask out PI,AI,UI
  OR AL,40h  // Enable periodic interrupts (PI=1) only
  OUT 71h,AL // Write into status register  B
  IN AL,71h  // Read to confirm write
  MOV byte ptr OldRegB2_70h,AL // Save old value

  IN AL,021h  // Make sure IRQ2 is not masked
  AND AL,0FBh // Write 0 to bit 2 of port 21h
  OUT 021h,AL // Write to port 21h

  IN AL,70h  // Set up "Read into status resister C"
  MOV AL,0Ch // Required for for "Write into port 71h"
  OUT 70h,AL
  IN AL,70h
  MOV AL,8Ch // 
  OUT 70h,AL
  IN AL,71h  // Read status register C 
             // (we do nothing with it)

  IN AL,70h  // Set up "Read into status resister C"
  MOV AL,0Dh // Required for for "Write into port 71h"
  OUT 70h,AL
  IN AL,70h
  MOV AL,8Dh
  OUT 70h,AL
  IN AL,71h  // Read status register D 
             // (we do nothing with it)


  STI
  POP AX

    } // asm
}

void RestoreInt70h() {
    asm{
         MOV AL,OldMaskA
         OUT 0A1h,AL

         IN AL,70h  // restore A status register
         MOV AL,0Ah
         OUT 70h,AL
         MOV AL,8Ah
         OUT 70h,AL
         IN AL,71h
         MOV AL,OldRegA2_70h
         OUT 71h,AL
         IN AL,71h

    } // asm
}

void SetNewLatch(int n)
{
    asm{
      CLI
      PUSH AX
      MOV AL,036h
      OUT 43h,AL
      MOV AX,n
      OUT 40h,AL
      MOV AL,AH
      OUT 40h,AL
      POP AX
      STI
    } 
}

void RestoreLatch() {
    asm{
     CLI
     PUSH AX
     MOV AL,036h
     OUT 43h,AL
     MOV AX,0
     OUT 40h,AL
     MOV AL,AH
     OUT 40h,AL
     POP AX
     STI
    } 
}
