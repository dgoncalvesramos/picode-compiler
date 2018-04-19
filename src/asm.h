/***************************************************************************/
/* main definitions                                                        */
#ifndef FILE_ASM_H
#define FILE_ASM_H
/***************************************************************************/
/* definition binaire de la VM pico                                        */

typedef unsigned int Taddr;
#define BYTEMEMSIZE  (1<<15)  // 32 kb
#define ADDR_UNDEF   ((Taddr)-1)

// Les instructions assembleur avec leurs codes opération
typedef enum _TinstOpCode {
    INST_ADD = 1*4,  INST_SUB= 2*4,  INST_MUL= 3*4,  INST_MOV  = 4*4,
    INST_JMP = 5*4,  INST_JE = 6*4,  INST_JL = 7*4,  INST_JLE  = 8*4,
    INST_EMP = 9*4,  INST_DEP=10*4,  INST_READ=11*4, INST_WRITE=12*4,
    INST_STOP=13*4
} TinstOpCode;

// The associated values are the 3 msb bits of the E.A description byte
typedef enum _TinstEAKind {
    EaImmediate=0x00,
    EaDirectMemory=0x20,   EaInDirectMemory=0x40,
    EaDirectRegister=0x80, EaInDirectRegister=0xA0
} TinstEAKind;

/***************************************************************************/
/* definition des différents opérateurs de l'ASM pico                      */

// les operateurs de comparaison
typedef enum _Topcmp {
     OPCMP_E,  OPCMP_NE
    ,OPCMP_G,  OPCMP_L
    ,OPCMP_GE, OPCMP_LE
} Topcmp;

/***************************************************************************/
#endif // FILE_ASM_H
