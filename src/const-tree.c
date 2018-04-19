/***************************************************************************/
/* expression that represents constant                                     */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "const-tree.h"

// constructeurs
extern Tconst* cst_newIdent(char* id)
{
	Tconst* cst = (Tconst*)malloc(sizeof(*cst));
	if(cst == NULL)
	{
		fprintf(stderr,"Allocation impossible\n");
		exit(EXIT_FAILURE);
	}
	cst->kind = CST_IDENT;
	cst->u.ident = id;
   cst->fg=0;
	cst->fd=0;
	return (Tconst*) cst ;
}
extern Tconst* cst_newNumber(int n)
{
	Tconst* cst = (Tconst*)malloc(sizeof(*cst));

	if(cst == NULL)
	{
	
		fprintf(stderr,"Allocation impossible\n");
		exit(EXIT_FAILURE);
	}

	cst->kind = CST_NUMBER;
	cst->u.integer = n;
	cst->fg=0;
	cst->fd=0;
	return (Tconst*) cst;
}

extern Tconst* cst_newBinOp(Tconst* l,TconstKind kind, Tconst* r)
{

	Tconst* cst = (Tconst*)malloc(sizeof(*cst));
	cst->kind = kind;
	cst->u.ident = 0;
   cst->fg=l;
	cst->fd=r;
	return cst ;
}

// desctructeur
extern void cst_delete(Tconst* cst)
{
	if(!cst) return;
	free(cst->fg);
	free(cst->fd);
	free(cst);
}


// Returns the evaluation of the cst expression.
// The value of the ident nodes are got using the
// ds_symGetAddr function.
extern int cst_eval(Tconst* cst)
{
	if(cst->kind == CST_IDENT) return ds_symGetAddr(cst->u.ident);
	else if(cst->kind == CST_PLUS) return cst_eval(cst->fg) + cst_eval(cst->fd);
	else if (cst->kind==CST_MINUS) return cst_eval(cst->fg) - cst_eval(cst->fd);
	else if (cst->kind==CST_TIME) return cst_eval(cst->fg) * cst_eval(cst->fd);
	else if (cst->kind==CST_LSH) return cst_eval(cst->fg) << cst_eval(cst->fd);
	else return cst->u.integer;
}

extern int cst_withIdent(Tconst* cst)
{
	if(cst->kind==CST_IDENT) return 1;
	else if(cst->kind==CST_NUMBER) return 0;
	else return cst_withIdent(cst->fg)+cst_withIdent(cst->fd);
	
}

