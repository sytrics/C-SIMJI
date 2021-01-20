//
// Created by sytrics on 13/01/2021.
//

#include <stdio.h>

#define NUM_REGS 32 //

signed regs[ NUM_REGS ]; // signed pour correspondre au CPL2

// notre programme en suite de binaires
unsigned program[] = { 0x1064, 0x11C8, 0x12FA, 0x2301, 0x3432, 0x0000 };

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
    instrNum = (instr & 0xF0000000) >> 28;
    regA     = (instr & 0x0F000000) >> 24;
    o1    = (instr & 0x007FFFF0) >>  4;
    o2     = (instr & 0x07FFFFF0) >>  4;
    a     = (instr & 0x00FFFFFF);
    n     = (instr & 0x0FFFFFFF);
    regB     = (instr & 0x0000000F);
    imm = (instr & 0x00800000) >> 21;
    immJ = (instr & 0x08000000) >> 25;

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

        case 0:
            /* add */
            printf( "add r%d #%d\n", regA, o1, regB );
            if (imm) {
                // valeure immédiate
                regs[ regB ] = regs[ regA ] + o1;
            }
            else {
                // adresse par registre
                regs[ regB ] = regs[ regA ] + regs[o1 & 0xF];

            }
            break;
        case 1:
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
        case 2:
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
        case 3:
            /*div*/
            printf("div r%d r%d r%d\n", regA, o1, regB );
            regs[ regB] = regs[regA] / o1;
            break;
        case 4:
            /*and*/
            printf("and r%d r%d r%d\n", regA, o1, regB );
            regs[ regB] = regs[regA] & o1;
            break;
        case 5:
            /*or*/
            printf("or r%d r%d r%d\n", regA, o1, regB );
            regs[ regB] = regs[regA] | o1;
            break;
        case 6:
            /*XOR*/
            printf("xor r%d r%d r%d\n", regA, o1, regB );
            regs[ regB] = regs[regA] ^ o1;
            break;
        case 7:
            /*shl*/
            printf("shl r%d r%d r%d\n", regA, o1, regB );

            break;
        case 8:
            /*shr*/
            printf("shr r%d r%d r%d\n", regA, o1, regB );

            break;
        case 9:
            /* slt */
            printf("slt r%d r%d r%d\n", regA, o1, regB );
            break;
        case 10:
            /* sle */
            printf("sle r%d r%d r%d\n", regA, o1, regB );

            break;
        case 11:
            /* seq */
            printf("seq r%d r%d r%d\n", regA, o1, regB );

            break;
        case 12:
            /*load*/
            printf("load r%d r%d r%d\n", regA, o1, regB );

            break;
        case 13:
            /*store*/
            printf("store r%d r%d r%d\n", regA, o1, regB );

            break;
        case 14:
            /*jmp*/
            printf("jmp r%d r%d\n", o2, regB);

            break;
        case 15:
            /*braz*/
            printf("XOR r%d r%d\n", regA, a);

            break;
        case 16:
            /*branz*/
            printf("XOR r%d r%d\n", regA, a);

            break;
        case 17:
            /*scall*/
            printf("XOR r%d\n", n);

            break;
        case 18:
            /* stop */
            printf( "halt\n" );
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
