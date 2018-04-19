/***************************************************************************/
/* symbol and symbol table                                                 */
#ifndef SYMBOL_H
#define SYMBOL_H

#include "asm.h"

/***************************************************************************/
/* symbol                                                                  */

typedef struct _Tsym {
    char* ident;
    Taddr addr;     // address of symbol or ADDR_UNDEF if not a valid address.
} Tsym;

/***************************************************************************/
/* symbol table                                                            */

//typedef void* TsymSet;

typedef struct _TsymSet TsymSet;

struct _TsymSet {
    Tsym* sym;
    TsymSet* fg;
    TsymSet* fd;
};

// constructor and destructor
extern TsymSet*    syms_new();
extern void       syms_delete(TsymSet* s);

// Adds to the s symbol set the symbol ln and sets its address to a.
// If symbol ln already exists in the s set, its address is overwritten.
extern void  syms_add(TsymSet** s, char*ln, Taddr a);

// Search into the s symbol set for the symbol, the ident of which is ln.
// Returns the pointer to symbol on success otherwise the null pointer. 
extern Tsym* syms_search(TsymSet* s, char*ln);

// return the address of ln symbol if it exists in s set otherwise the value ADDR_UNDEF.
extern Taddr syms_getAddr(TsymSet* s, char*ln);

// walk in the symSet table
extern void  syms_walk(TsymSet* s);


#endif // FILE_SYMBOL_H
