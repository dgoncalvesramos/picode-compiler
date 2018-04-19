/***************************************************************************/
/* expression that represents constant                                     */
#ifndef FILE_CONST_TREE_H
#define FILE_CONST_TREE_H

#include "asm.h"

typedef enum _TconstKind {
    CST_NUMBER, CST_IDENT,
    CST_PLUS, CST_MINUS, CST_TIME,
    CST_LSH, CST_RSH
} TconstKind;

typedef struct _Tconst{
	 TconstKind kind;
	  union {
        int integer;
        char* ident;
    }u;
	struct _Tconst* fg,*fd;
}Tconst;



// constructeur
extern Tconst* cst_newIdent(char* id);
extern Tconst* cst_newNumber(int  n);
extern Tconst* cst_newBinOp(Tconst* l,TconstKind, Tconst* r);

// desctructeur
extern void cst_delete(Tconst* cst);

// Returns the evaluation of the cst expression.
// The value of the ident nodes are got using the
// ds_symGetAddr function.
extern int cst_eval(Tconst* cst);

// Returns the number of ident nodes of cst expression
extern int cst_withIdent(Tconst* cst);
#endif // FILE_CONST_TREE_H
