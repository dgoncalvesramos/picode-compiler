#ifndef FILE_YACC_SEMANTIC_H
#define FILE_YACC_SEMANTIC_H
#include "asm.h"
#include "datastructure.h"
#include "operande.h"

/**********************INST WITH OPERANDS*******************/
extern void startYaccInstruction(TinstOpCode code, int sz,Toperande* opDest,ToperandeSet* listOpSrc, Tconst* cstAis);
extern void startYaccIf(int sz, Toperande* opDest, Topcmp opcmp, Toperande* opSrc, char* branch_addr_true, char* branch_addr_false);
/****************************INST WITHOUT OPERANDS******************************/
extern void startYaccJmp(TinstOpCode code, Tconst* cst_branch);
extern void startYaccCall(TinstOpCode code, Tconst* cstr_branch, Tconst* AisOptionnel);
extern void startYaccRet(TinstOpCode code);
extern void startYaccStop(TinstOpCode code);
/***************************PRIMITIVES*************************************************/
extern void startYaccLong(Tconst* cst);
extern void startYaccWord(Tconst* cst);
extern void startYaccByte(Tconst* cst);
extern void startYaccStr(char* str);
/**************************************************************************************/
#endif //FILE_YACC_SEMANTIC
