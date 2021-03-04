//
// Created by sytrics on 13/01/2021.
//

#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <time.h>
#define NUM_REGS 32 //
#define MAX_SIZE_PROGRAM 1024 // taille max de program

//TODO => ajout de la mémoire 
//TODO => prise en compte du jump dans le fetching 

signed regs[ NUM_REGS ]; // signed pour correspondre au CPL2
signed scall; // temp variable for scall 
// notre programme en suite de binaires
long program[MAX_SIZE_PROGRAM];

signed memory[MAX_SIZE_PROGRAM]; 
/* program counter */
int pc = 0;
int Benchnum = 0; 
double Benchtime = 0; 
clock_t before; 

/* fetch the next word from the program */
int fetch()
{
    Benchnum ++; 
    return program[ pc++ ];
}

/* instruction fields */
int instrNum=0;
int regA= 0;
int o1= 0;
int o2=0;
int regB=0;
int imm=0;
int immJ=0;
int n=0;
int a=0;

/* decode a word */
void decode( int instr )
{
    instrNum = (instr & 0xF8000000) >> 27;
    regA     = (instr & 0x03C00000) >> 22;
    o1    = (instr & 0x001FFFE0) >>  5;
    o2     = (instr & 0x01FFFFE0) >>  5;
    a     = (instr & 0x003FFFFF);
    n     = (instr & 0x07FFFFFF);
    regB     = (instr & 0x0000001F);
    imm = (instr & 0x00200000) >> 21;
    immJ = (instr & 0x04000000) >> 26;

}

/* the VM runs until this flag becomes 0 */
int running = 1;

int CPL2todec(int nombre, int taille){
    // convertit n'importe quel nombre CPL2 en int signé
    int  bit = nombre >> (taille-1);
    if (bit == 1) {
        return -1 * (nombre - (1 << (taille-1)) );

    }
    else {
        return nombre;
    }
}

int dectoCPL2(int nombre, int taille){
    // converti n'importe quel int signé en binaire CPL2
    if (nombre >=0) {
        return nombre;
    }
    else {
        return (1<<(taille-1))-nombre;
    }
}

/* evaluate the last decoded instruction */
void eval()
{
    o1 = CPL2todec(o1, 16);
    
    switch( instrNum )
    {

        case 1:
            /* add */
            printf( "add r%d %d r%d imm=%d\n", regA, o1, regB,imm );
            if (imm==1) {
                // valeure immédiate
                regs[ regB ] = regs[ regA ] + o1;

            }
            else {
                // adresse par registre
                regs[ regB ] = regs[ regA ] + regs[o1 & 0x1F];


            }
            break;
        case 2:
            /* sub */
            printf( "sub r%d %d r%d imm=%d\n", regA, o1, regB,imm );
            if (imm==1) {
                // valeure immédiate
                regs[ regB ] = regs[ regA ] - o1;

            }
            else {
                // adresse par registre
                regs[ regB ] = regs[ regA ] - regs[o1 & 0x1F];


            }
            break;
        case 3:
            /* mul */
            printf( "mul r%d %d r%d imm=%d\n", regA, o1, regB,imm );
            if (imm==1) {
                // valeure immédiate
                regs[ regB ] = regs[ regA ] * o1;

            }
            else {
                // adresse par registre
                regs[ regB ] = regs[ regA ] * regs[o1 & 0x1F];


            }
            break;
        case 4:
            /* div */
            printf( "div r%d %d r%d imm=%d\n", regA, o1, regB,imm );
            if (imm==1) {
                // valeure immédiate
                regs[ regB ] = regs[ regA ] / o1;

            }
            else {
                // adresse par registre
                regs[ regB ] = regs[ regA ] / regs[o1 & 0x1F];


            }
            break;
        case 5:
            /* and */
            printf( "and r%d %d r%d imm=%d\n", regA, o1, regB,imm );
            if (imm==1) {
                // valeure immédiate
                regs[ regB ] = regs[ regA ] & o1;

            }
            else {
                // adresse par registre
                regs[ regB ] = regs[ regA ] & regs[o1 & 0x1F];


            }
            break;
        case 6:
            /* or */
            printf( "or r%d %d r%d imm=%d\n", regA, o1, regB,imm );
            if (imm==1) {
                // valeure immédiate
                regs[ regB ] = regs[ regA ] | o1;

            }
            else {
                // adresse par registre
                regs[ regB ] = regs[ regA ] | regs[o1 & 0x1F];


            }
            break;
        case 7:
            /* xor */
            printf( "xor r%d %d r%d imm=%d\n", regA, o1, regB,imm );
            if (imm==1) {
                // valeure immédiate
                regs[ regB ] = regs[ regA ] ^ o1;

            }
            else {
                // adresse par registre
                regs[ regB ] = regs[ regA ] ^ regs[o1 & 0x1F];


            }
            break;
        case 8:
            /*shl*/
            printf("shl r%d r%d r%d\n", regA, o1, regB );
            if (imm==1) {
                // valeure immédiate
                regs[ regB ] = regs[ regA ] << o1;

            }
            else {
                // adresse par registre
                regs[ regB ] = regs[ regA ] << regs[o1 & 0x1F];


            }
            break;
        case 9:
            /*shr*/
            printf("shr r%d r%d r%d\n", regA, o1, regB );
            if (imm==1) {
                // valeure immédiate
                regs[ regB ] = regs[ regA ] >> o1;

            }
            else {
                // adresse par registre
                regs[ regB ] = regs[ regA ] >> regs[o1 & 0x1F];


            }
            break;
        case 10:
            /* slt */
            printf("slt r%d r%d r%d\n", regA, o1, regB );
            if (imm==1) {
                // valeure immédiate
                if (regs[ regA ] < o1) {regs[regA] = 1;}
                     
                else {regs[regA] = 0;}
            
            }
            else {
                // adresse par registre
                
                if (regs[ regA ] < regs[o1 & 0x1F]) {regs[regA] = 1;}
                     
                else {regs[regA] = 0;}
            

            }
            break;
            
        case 11:
            /* sle */
            printf("sle r%d r%d r%d\n", regA, o1, regB );
            if (imm==1) {
                // valeure immédiate
                if (regs[ regA ] <= o1) {regs[regA] = 1;}
                     
                else {regs[regA] = 0;}
            
            }
            else {
                // adresse par registre
                
                if (regs[ regA ] <= regs[o1 & 0x1F]) {regs[regA] = 1;}
                     
                else {regs[regA] = 0;}
            

            }
            break;
        case 12:
            /* seq */
            printf("seq r%d r%d r%d\n", regA, o1, regB );
            if (imm==1) {
                // valeure immédiate
                if (regs[ regA ] == o1) {regs[regA] = 1;}
                     
                else {regs[regA] = 0;}
            
            }
            else {
                // adresse par registre
                
                if (regs[ regA ] == regs[o1 & 0x1F]) {regs[regA] = 1;}
                     
                else {regs[regA] = 0;}
            

            }
            break;
        case 13:
            /*load*/
            printf("load r%d r%d r%d\n", regA, o1, regB );
            if (imm==1) {
                regs[ regB ] = memory[regs[regA]+o1];
            } 
            else {
                regs[ regB ] = memory[regs[regA]+regs[o1 & 0x1F]];
            }
            break;
        case 14:
            /*store*/
            printf("store r%d r%d r%d\n", regA, o1, regB );
            if (imm==1) {
                memory[regs[regA]+o1] = regs[ regB ];
            } 
            else {
                memory[regs[regA]+regs[o1 & 0x1F]] = regs[ regB ];
            }
            break;
        case 15:
            /*jmp*/
            if (immJ == 1) {
                printf("jmp %d r%d\n", o2, regB);
            } else {
                printf("jmp r%d r%d\n", o2, regB);
            }
            pc = o2;
            if (regB != 0) {
                regs[regB] = o2;
            }
            


            break;
        case 16:
            /*braz*/
            printf("braz r%d %d\n", regA, a);
            if (regs[regA]==0) pc = a;
            break;
        case 17:
            /*branz*/
            printf("branz r%d %d\n", regA, a);
            if (regs[regA]!=0) pc = a;
            break;
        case 18:
            /*scall*/
            printf("SCALL r%d\n", n);
            if (n!=0) {printf("r%d === > %08X \n",n ,regs[n]);} 
            else {
                // appel systeme et stockage dans r1
                // arret du compteur
                Benchtime += (clock()-before); 
                printf("enter N: "); 
                scanf("%d", &scall);
                regs[1] = scall;
                // reprise du compteur 
                before = clock();
            }
            // time delta 

            break;
        case 0:
            /* stop */
            printf( "stop\n" );
            running = 0;
            break;
        default:
            /*unknown command*/
            printf( "command not found, halt\n" );
            running = 0;
            break;
    }
}
void init() {
    // start counter 
    before = clock();

    FILE *instructions;
    char buffer[24] ;
    int stop=1;
    char *line;
    char *ptr;
    long inst;
    long memory_index;
    char delim[] = " \n";
    instructions = fopen("bin/instruction.bin", "r");
    if (fgets(buffer, sizeof(char)*23, instructions)==NULL) stop=0;
    while (stop) {
        line = strtok (buffer, delim);
        memory_index = strtol (line,&ptr, 0);
        line = strtok (NULL, delim);
        inst = strtol (line,&ptr,0);
        program[memory_index] = inst;
        if (fgets(buffer, sizeof(char)*23, instructions)==NULL) stop=0;
    }
    fclose(instructions);
    
    

}

/* display all registers as 4-digit hexadecimal words */
void showRegs()
{
    int i;
    printf( "regs = " );
    for( i=0; i<NUM_REGS; i++ )
        printf( "%08X ", regs[ i ] );
    printf( "\n" );
}

void run()
{
    while( running )
    {
        showRegs();
        int instr = fetch();
        decode( instr );
        eval();
    }
    showRegs();
    Benchtime += (clock()-before); 
}

int main( int argc, const char * argv[] )
{
    init();
    run();
    printf("==================END=================\n");
    printf("Benchmark results : \n");
    printf("ops complexity : %d operations \n", Benchnum);
    printf("number of CPU ticks %.0f \n", Benchtime);
    printf("time complexity : %.3f ms \n", Benchtime*1000/CLOCKS_PER_SEC );
    return 0;
}
