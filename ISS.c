//
// Created by sytrics on 13/01/2021.
//

#include <stdio.h>

#define NUM_REGS 32 //

signed regs[ NUM_REGS ]; // signed pour correspondre au CPL2

// notre programme en suite de binaires
unsigned program[] = {0x08200021,0x98000000};

/* program counter */
int pc = 0;

/* fetch the next word from the program */
int fetch()
{
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
        return -1 * (nombre - 1 << (taille-1));
    }
    else {
        return nombre;
    }
}

int dectoCPL2(int nombre, int taille){
    // convertit n'importe quel int signé en binaire CPL2
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
    o2 = CPL2todec(o2, 23);
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
            printf( "sub r%d r%d r%d\n", regA, o1, regB );
            if (imm) {
                // valeure immédiate
                regs[ regB ] = regs[ regA ] - o1;
            }
            else {
                // adresse par registre
                regs[ regB ] = regs[ regA ] - regs[o1 & 0xF];

            }
            break;
        case 3:
            /*mult*/
            printf( "mult r%d r%d r%d\n", regA, o1, regB );
            if (imm) {
                // valeure immédiate
                regs[ regB ] = regs[ regA ] * o1;
            }
            else {
                // adresse par registre
                regs[ regB ] = regs[ regA ] * regs[o1 & 0xF];

            }
            break;
        case 4:
            /*div*/
            printf("div r%d r%d r%d\n", regA, o1, regB );
            regs[ regB] = regs[regA] / o1;
            break;
        case 5:
            /*and*/
            printf("and r%d r%d r%d\n", regA, o1, regB );
            regs[ regB] = regs[regA] & o1;
            break;
        case 6:
            /*or*/
            printf("or r%d r%d r%d\n", regA, o1, regB );
            regs[ regB] = regs[regA] | o1;
            break;
        case 7:
            /*XOR*/
            printf("xor r%d r%d r%d\n", regA, o1, regB );
            regs[ regB] = regs[regA] ^ o1;
            break;
        case 8:
            /*shl*/
            printf("shl r%d r%d r%d\n", regA, o1, regB );

            break;
        case 9:
            /*shr*/
            printf("shr r%d r%d r%d\n", regA, o1, regB );

            break;
        case 10:
            /* slt */
            printf("slt r%d r%d r%d\n", regA, o1, regB );
            break;
        case 11:
            /* sle */
            printf("sle r%d r%d r%d\n", regA, o1, regB );

            break;
        case 12:
            /* seq */
            printf("seq r%d r%d r%d\n", regA, o1, regB );

            break;
        case 13:
            /*load*/
            printf("load r%d r%d r%d\n", regA, o1, regB );

            break;
        case 14:
            /*store*/
            printf("store r%d r%d r%d\n", regA, o1, regB );

            break;
        case 15:
            /*jmp*/
            printf("jmp r%d r%d\n", o2, regB);

            break;
        case 16:
            /*braz*/
            printf("XOR r%d r%d\n", regA, a);

            break;
        case 17:
            /*branz*/
            printf("XOR r%d r%d\n", regA, a);

            break;
        case 18:
            /*scall*/
            printf("XOR r%d\n", n);

            break;
        case 19:
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

/* display all registers as 4-digit hexadecimal words */
void showRegs()
{
    int i;
    printf( "regs = " );
    for( i=0; i<NUM_REGS; i++ )
        printf( "%04X ", regs[ i ] );
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
}

int main( int argc, const char * argv[] )
{
    run();
    return 0;
}
