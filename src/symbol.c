#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "symbol.h"

// constructor and destructor
extern TsymSet* syms_new()
{
	return NULL ;
}

extern void syms_delete(TsymSet* s)
{
	if (s!=NULL)
	{
		syms_delete(s->fg);
		syms_delete(s->fd);
		free(s->sym);
	}
}

// Adds to the s symbol set the symbol ln and sets its address to a.
// If symbol ln already exists in the s set, its address is overwritten.
extern void syms_add(TsymSet** s, char* ln, Taddr a)
{


	TsymSet* newTsymSet = (TsymSet*)malloc(sizeof(TsymSet));
	newTsymSet->sym = (Tsym*)malloc(sizeof(Tsym));
	newTsymSet->sym->ident = (char*)malloc(strlen(ln)*sizeof(char)+1);
	newTsymSet->fg = 0;
	newTsymSet->fd = 0;
	
	strcpy(newTsymSet->sym->ident, ln);
	newTsymSet->sym->addr = a;
	if(*s==0)
	{
		*s=newTsymSet;
		return;
	}
	
	TsymSet* tmpTsymSet = *s;

	if(strcmp(ln, tmpTsymSet->sym->ident)==0)
	{
		tmpTsymSet->sym->addr = a;
		return;
	}
	else if(strcmp(ln, tmpTsymSet->sym->ident)>0)
		syms_add(&tmpTsymSet->fd, ln, a);
	else 
		syms_add(&tmpTsymSet->fg, ln, a);
}

// Search into the s symbol set for the symbol, the ident of which is ln.
// Returns the pointer to symbol on success otherwise the null pointer. 
extern Tsym* syms_search(TsymSet* s, char* ln)
{
	if(s==0)
	{
		return 0;
	}
	
	if(strcmp(ln, s->sym->ident)==0)
	{
		return s->sym;
	}
	else if(strcmp(ln, s->sym->ident)>0)
		syms_search(s->fd, ln);
	else 
		syms_search(s->fg, ln);
}

// return the address of ln symbol if it exists in s set otherwise the value ADDR_UNDEF.
extern Taddr syms_getAddr(TsymSet* s, char* ln)
{
	Tsym* sym ;
	if((sym=syms_search(s,ln))==0) return ADDR_UNDEF;
	return sym->addr;
}

// walk in the symSet table
extern void syms_walk(TsymSet* s)
{
	if(!s) {return;}
	printf("SYMBOL : %20s ADDR : %5d\n",s->sym->ident,s->sym->addr);
	syms_walk(s->fg);
	syms_walk(s->fd);
}

