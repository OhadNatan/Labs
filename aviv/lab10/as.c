#include <dos.h>
#include <stdio.h>
#define ON (1)
#define OFF (0)
#define N 200
#define ENT (28)
/* ---------------------------------------------------------------------------
   --	Program that will check if by pressing 2 keys how many time it passed until
   --    clicking on the second button
   ---------------------------------------------------*-----------------------*/
void interrupt(*int9save)(void);
void interrupt(*int70save)(void);
void bubble_sort();
void ChangeSpeaker(int status);
void Sound(long int hertz);
void set_median();
int local_flag = 1, x71h1 = 0, x71h2 = 0, x71h3;
char old_0A1h_mask, old_70h_A_mask;
volatile int  loop_realeser = 0;        /* variable to stop the main program until clicking on keybord*/
volatile int first_time = 0;            /* var to know the enter realesed*/
volatile int un_print = 0;              /* var to print the output for  tests*/
volatile int val = ENT;                  /* val of the enter */
volatile int checker = ENT;              /* var  used first time in main function only*/
volatile long int count = 1;              /* var to count the interupts between 2 key preses*/
volatile long int times_req[N] = { 1 }; /* array to save all the time between 2 clicks*/
volatile int index_of_arr = 0;          /* var to save the next available location to store values in arr*/
volatile int flag_first = 0;            /* flag for the first time clicking*/
volatile long int total_time_counte = 0;  /* var to store the total time passed of the program*/
double med;                             /* Save the median */
void interrupt newint70(void)
{
    count++;
    total_time_counte++;
    asm{
        MOV AL,20h
        OUT 20h,AL
        PUSHF
        CALL DWORD PTR int70save
    };
} // newint70(void)




/*
--------------------------------------------------------------------------------------
---                                                                                         ---
---    Keybord interupt- When user press the button:
        * Need to save how many time it took from the last pressing to this one
        * Need to change the latch of the Speakers to the amount of interupts
        * Need to reset the counter and start counting again                        ---
---                                   ---
---                                   ---
--------------------------------------------------------------------------------------
*/
void interrupt newint9(void)
{
    // if this is the first time running this code, we dont want to change anything
    if (first_time != 0) {
        asm{
               MOV AX,0
               IN AL,60h
               PUSH AX
               IN   AL,61h
               OR   AL,80h
               OUT  61h,AL
               AND  AL,7Fh
               OUT  61h,AL

               POP  AX
               TEST AL,80h
               JNZ RealesKey //im 1 its mean We are now realese the key
               CMP BYTE PTR ENT,AL
               JE SameValueAsBefore //if im here its mean that the user pressed ENTER
               CMP BYTE PTR val,AL//Check if the value is like the old value
               JE SameValueAsBefore//Im the same as i was
               MOV BYTE PTR val,AL//Move the value to the val
        }
            /*-------------------------------------
                If we are here its mean:
                    1. The interrupt that has been called is called when the user pressed
                    2. The scan code of the key that has been pressed is different then the
                    scan code of the key that has been presse before.
              --------------------------------------*/
       
                times_req[index_of_arr] = count;
                index_of_arr += 1;
                count = 0;
        
        
        

    }
    if (flag_first == 0) {
        int70save = getvect(0x70);
        setvect(0x70, newint70);
        flag_first = 1;
        loop_realeser = 1;
    }

    first_time = 1;
    //If got here its mean that the key is 1 and i don't want to print it 
    un_print = 1;
SameValueAsBefore:
    loop_realeser = 1;
RealesKey:
    // doing the original Interupt 9
    asm{
        PUSHF
        CALL DWORD PTR int9save
    }
    loop_realeser = 1;
} // newint9(void)




/*--------------------------------------------------
Median Calculator function, will be  called in the end of the program .*/

void set_median() {
    med = (times_req[N / 2 - 1] + times_req[N / 2]) / 2;
}



/*------------------------------------------------
 ChangeSpeaker - Turn speaker on or off. */

void ChangeSpeaker(int status)
{
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

} /*--ChangeSpeaker( )----------*/



/*--------------------------------------------------------------
    Sound- The function sound will be changing all the time
    based on what are the difranace interput time between some of the key presses. */
void Sound(long int hertz)
{
    unsigned divisor; 
    if (hertz != 0)
        if ((int)(divisor / hertz) > 1069)
            divisor = 1069;
    else
        divisor = 1193180L / hertz;
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

}

/*--------------------------------------------------------
 bubble_sort- Simple buble sort function, to sort the values from the arr. */
void bubble_sort() {
    int write, sort;
    int temp;
    for (write = 0; write < N; write++)
    {
        for (sort = 0; sort < N - 1; sort++)
        {
            if (times_req[sort] > times_req[sort + 1])
            {
                temp = times_req[sort + 1];
                times_req[sort + 1] = times_req[sort];
                times_req[sort] = temp;
            }
        }
    }
} // bubble_sort()


void set_IRQ8() {

    asm{
       CLI         // Disable interrupts
       PUSH AX     // Interrupt may occur while updating

       IN AL,0A1h  // Make sure IRQ8 is not masked
       MOV old_0A1h_mask,AL
       AND AL,0FEh // Set bit 0 of port 0A1 to zero
       OUT 0A1h,AL //

       IN AL,70h   // Set up "Write into status register A"
       MOV AL,0Ah  //
       OUT 70h,AL  //
       MOV AL,8Ah  //
       OUT 70h,AL  //
       IN AL,71h   //
       MOV BYTE PTR x71h1,AL  // Save old value
       MOV old_70h_A_mask,AL
       AND AL,11110000b // Change only Rate
       OR AL,0110b // Make sure it is Rate =0110 (1Khz)
       OUT 71h,AL  // Write into status register A
       IN AL,71h   // Read to confirm write



       IN AL,70h  // Set up "Write into status register B"
       MOV AL,0Bh //
       OUT 70h,AL //
       MOV AL,8Bh //
       OUT 70h,AL //
       IN AL,71h  //
       MOV BYTE PTR x71h2,AL // Save Old value
       AND AL,8Fh // Mask out PI,AI,UI
       OR AL,40h  // Enable periodic interrupts (PI=1) only
       OUT 71h,AL // Write into status register  B
       IN AL,71h  // Read to confirm write
       MOV byte ptr x71h3,AL // Save old value

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

/*
--------------------------------------------------------------------------------------
---                                                                                         ---
---    MAIN FUNCTION - will loop until the end of life, will wait for key press every time  ---
---    If key pressed- will print a message and check in the interupt if this key  the same ---
---    If key reeleased - Will not do anything, will ignore                                 ---
--------------------------------------------------------------------------------------
*/

int main(void)
{
    int i, flag_for_ent = 0;
    
    int9save = getvect(9);
    setvect(9, newint9);
    set_IRQ8();

    ChangeSpeaker(ON);

    while (index_of_arr < N)
    {
        /* We are running in a while loop (line 256) until our array has been filled.
             The array is filled only when the user is pressing, the key only then the
             index will raise by one. */
        while (loop_realeser != 1) {}
        if (flag_for_ent == 0 && val == ENT) // if the state is true that mean that we are in release the enter when first program start
            flag_for_ent = 1;
        else {
            Sound(times_req[index_of_arr - 1]);
            if (val == ENT)
                break;
            //printf("I AM Index --> %d\n", index_of_arr - 1); THIS IS TO CHECK THE INDEX OF THE INSERT

        }



        if (un_print == 0)
            //printf("The Key scan code is : %d\n", val);
            un_print = 0;

        if (checker != val) {
            //printf("The keys pressed are not the same! GOOD \n");

            checker = val;

        }
        loop_realeser = 0;
    }
    setvect(9, int9save);
   
    ChangeSpeaker(OFF);
    printf("timed\n");
    for (i = 0; i < N; i++) {
        if (times_req[i] != 0)
            printf("%ld ", times_req[i]);
    }

    bubble_sort();
    printf("\nSorted:\n");
    for (i = 0; i < N; i++)
    {
        if (times_req[i] != 0)
            printf("%ld ", times_req[i]);
    }

    printf("\nmax_time = %ld / 1069\nmin_time = %ld / 1069", times_req[N - 1], times_req[0]);
    set_median();
    printf("\nmed_time:%.2f", med);
    printf("\ntotal time  = %ld / 1069", total_time_counte);
    
    return(0);
} //main

