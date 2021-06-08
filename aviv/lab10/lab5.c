#include <stdio.h>
#include <stdlib.h>
#include <dos.h>

volatile int count = 0, index, old_scan_code = 0, first_push = 0;
char flag = '1';
int intevalArray[200] = {0};

void interrupt (*int9old)(void);
void interrupt (*int8old)(void);

void change_pit(int divNum){
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

void restore_pit(){
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
   count++;
   int8old();
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

    if(index == 200)
        flag = '0'; 

    if(is_pushed == 0){
        if (scan_code == 45)
            flag = '0'; 
        
        if(old_scan_code == 0){
            index = 0;
            count = 0;
            old_scan_code = scan_code;
        }
        else if(scan_code != old_scan_code){
            intevalArray[index++] = count;
            count = 0;
            old_scan_code = scan_code;
        }
    }
    int9old();
}

int compare_function(const void *a,const void *b) {
    int *x = (int *) a;
    int *y = (int *) b;
    return *x - *y;
}

void print_arr(){
    int i;
    for(i = 0; i < index ; i++)
        if(intevalArray[i] != 0)    
            printf("%d ", intevalArray[i]);
}

void main(){
    int i, sum;
    change_pit(700);
    int8old = getvect(8);
    setvect(8,int8handler);
    printf("Please enter string (up to 200 characters, end by pressing X)\n");
    int9old = getvect(9);
    setvect(9,int9handler);
    while(flag != '0'){
        asm{
            MOV AH,0
            INT 16h
        }
    }
    setvect(8,int8old);
    setvect(9,int9old);
    restore_pit();
    printf("timed:\n");
    print_arr();
    printf("\nSorted:\n");
    qsort(intevalArray, index, sizeof(int), compare_function);
    print_arr();
    printf("\nmax_time = %d / %d secs\n", intevalArray[index-1], 1069);
    printf("min_time = %d / %d secs\n", intevalArray[0], 1069 );
    printf("med_time = %d \n", intevalArray[(index-1)/2]);
    
    for(i = 0; i < index ; i++)
        if(intevalArray[i] != 0)    
            sum += intevalArray[i];
    
    printf("total_time = %d / 1069", sum);

    

    

}
