/***************************************************************************/
/* operandes et ensembles d'opérandes                                      */
#ifndef FILE_OPERANDE_H
#define FILE_OPERANDE_H

#include "asm.h"
#include "const-tree.h"

/***************************************************************************/
/* the different effective addresses                                       */

typedef struct _Toperande {
    TinstEAKind kind;  // 4 msb of binary code of E.A is: kind+pp
    int  pp;           // 0x40:pre-increment 0:no-increment 0x20:post-increment
    union {
        Tconst* val;  // for immediate and memory E.A
        int     reg;  // range [0:15]
    } u;
} Toperande;

// constructor of différent E.A.
extern Toperande* ea_newImmediat(Tconst* val);
extern Toperande* ea_newDirectMemory(Tconst* val);
extern Toperande* ea_newInDirectMemory(Tconst* val);
extern Toperande* ea_newDirectRegister(int reg);
extern Toperande* ea_newInDirectRegister(int reg);
extern Toperande* ea_newInDirectRegisterPreInc(int reg);
extern Toperande* ea_newInDirectRegisterPostInc(int reg);
//destructor
extern void ea_delete(Toperande* ea);
// returns the byte size for coding the E.A. ea for an instruction
// working with word of sz bytes
// Caution: sz must be 1, 2 or 4.
extern int ea_length(const Toperande* ea, int sz);

// writes into the memory pointed to by buf the binary code of
// the E.A. ea for an instruction working with word of sz bytes.
// When the last parameter is set to 1 the bit 4 of first byte is
// set (last operand of instruction).
// When the last parameter is set to 0 the bit 4 of first byte is
// reset (not the last operand of instruction).
// Caution: sz must be 1, 2 or 4.
// Caution: the buf buffer must be large enough.
// Caution: The last parameter must be either 0 or 1.
// returns the number of byte written into the buffer.
extern int ea_code(char* buf, const Toperande* ea, int sz, int last);

// prints into the stdout stream the ea operande under human format
extern void ea_codeHum(const Toperande* ea, int sz, int last);

/***************************************************************************/
/* effective address set                                                   */

#define EA_MaxNbOpe 30

typedef struct _ToperandeSet { // FIXME: must be changed to dynamic allocation
    int        nb;   // the number of operande int t
    Toperande* t[EA_MaxNbOpe];
} ToperandeSet;

extern ToperandeSet* eas_new();
extern void eas_delete(ToperandeSet*eas);
// return: 1 for full otherwise 0.
extern int eas_isFull(const ToperandeSet*eas);
// caution: set must not be full.
extern ToperandeSet* eas_add(ToperandeSet*eas, Toperande* ea);
// returns the byte size for coding all the E.A. of eas for an instruction
// working with word of sz bytes
// Caution: sz must be 1, 2 or 4.
extern int eas_length(const ToperandeSet* eas, int sz);

// writes into the memory pointed to by buf the binary code of
// all the E.A. of eas for an instruction working with word of sz bytes.
// Caution: sz must be 1, 2 or 4.
// Caution: the buf buffer must be large enough.
// returns the number of byte written into the buffer.
extern int eas_code(char* buf, const ToperandeSet* eas, int sz);
// prints on stdout the operande set under human format
extern void eas_codeHum(const ToperandeSet* eas, int sz);
#endif // FILE_OPERANDE_H
