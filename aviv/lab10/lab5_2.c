#include <stdio.h>
#include <stdlib.h>
#include <dos.h>

#define ON (1)
#define OFF (0)

volatile int old_0A1h_mask, old_70h_A_mask, x71h1, x71h2, x71h3; //parameters to remeber the original port values
volatile int count, index, old_scan_code, first_push; //parametrs to count
char flag; //flag to stop the main loop
int intevalArray[200] = {0}; //arrray of intervals


void interrupt (*int9old)(void);//save the original interupt
void interrupt (*int_70h_old)(void);//save the original interupt


//from the lab 
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
//from the lab
void Sound(int hertz)
{
    unsigned divisor;
    
    divisor = 1193180L / hertz;

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
//fro, the lab
void NoSound(void)
{
    ChangeSpeaker(OFF);
} /*--NoSound( )------*/
//change the rtc 70h ratio
void change_RTC()
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
//how to handle the 70h interupt
void interrupt int_0x70_handler(void)
{
    int_70h_old();
    count++;
}
//how to handle the new 9 interupt
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

    if (index == 200)
        flag = '0';

    if (is_pushed == 0)
    {
        if (scan_code == 45)
            flag = '0';

        if (old_scan_code == 0)
        {
            index = 0;
            count = 0;
            old_scan_code = scan_code;
            Sound(1);
        }
        else if (scan_code != old_scan_code)
        {
            Sound(intevalArray[index - 1]);
            intevalArray[index++] = count;
            count = 0;
            old_scan_code = scan_code;
        }
    }
    int9old();
}
//compare between 2 ints function
int compare_function(const void *a, const void *b)
{
    int *x = (int *)a;
    int *y = (int *)b;
    return *x - *y;
}
//function to pront an array
void print_arr()
{
    int i;
    for (i = 0; i < index; i++)
        printf("%d ", intevalArray[i]);
}

void restore_RTC() {
    asm{
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
void main()
{
    int i;
    long int sum = 0;
    count =  index =  old_scan_code =  first_push = 0;
    flag = '1';
    change_RTC(); //change the rtc retio
    int_70h_old = getvect(0x70); //save the interupt 70h
    setvect(0x70, int_0x70_handler); //change the 70h interupt
    printf("Please enter string (up to 200 characters, end by pressing X)\n");
    int9old = getvect(9); //save the interupt 9
    setvect(9, int9handler); //change the  interupt 9
    while (flag != '0') //wait to the user done push the keyboard
    {
        asm {
            MOV AH,0
            INT 16h
        }
    }
    restore_RTC(); //return the original rtc
    setvect(0x70, int_0x70_handler); //reverse the 70h to original
    setvect(9, int9old);//reverse the 9 to original
    printf("timed:\n");
    print_arr();
    printf("\nSorted:\n");
    qsort(intevalArray, index, sizeof(int), compare_function); //stdlib function to sort the array
    print_arr();
    printf("\nmax_time = %d / %d secs\n", intevalArray[index - 1], 1069); //get the max value from the array
    printf("min_time = %d / %d secs\n", intevalArray[0], 1069); //same with min
    printf("med_time = %d \n", intevalArray[(index - 1) / 2]); //med value calculation

    for (i = 0; i < index; i++) //sum all the values
        sum += intevalArray[i];

    printf("total_time = %ld / 1069", sum);

    NoSound(); //return the sound system
}
