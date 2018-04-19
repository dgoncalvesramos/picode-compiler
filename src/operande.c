
/***************************************************************************/
/* operandes et ensembles d'opérandes                                      */

#include <stdio.h>
#include <stdlib.h>

#include "operande.h"
// constructor of différent E.A.
static Toperande* ea_new(TinstEAKind kd, int pp)
{
    Toperande* ret = (Toperande*)malloc(sizeof(*ret));
    ret->kind  = kd;
    ret->pp    = pp;
    return ret;
}
extern void ea_delete(Toperande* ea)
{
    switch (ea->kind) {
        case EaImmediate:
        case EaDirectMemory:
        case EaInDirectMemory:
            cst_delete(ea->u.val);
            break;
    }
    free(ea);
}

extern Toperande* ea_newImmediat(Tconst* val)
    { Toperande* ret = ea_new(EaImmediate,0);    ret->u.val=val; return ret; }

extern Toperande* ea_newDirectMemory(Tconst* val)
    { Toperande* ret = ea_new(EaDirectMemory,0); ret->u.val=val; return ret; }

extern Toperande* ea_newInDirectMemory(Tconst* val)
    { Toperande* ret = ea_new(EaInDirectMemory,0); ret->u.val=val; return ret; }

extern Toperande* ea_newDirectRegister(int reg)
    { Toperande* ret = ea_new(EaDirectRegister,0); ret->u.reg=reg; return ret;}

extern Toperande* ea_newInDirectRegister(int reg)
    { Toperande* ret = ea_new(EaInDirectRegister,0); ret->u.reg=reg; return ret; }

extern Toperande* ea_newInDirectRegisterPreInc(int reg)
    { Toperande* ret = ea_new(EaInDirectRegister,0x40); ret->u.reg=reg; return ret; }

extern Toperande* ea_newInDirectRegisterPostInc(int reg)
    { Toperande* ret = ea_new(EaInDirectRegister,0x20); ret->u.reg=reg; return ret; }

// returns the byte size for coding the E.A. ea for an instruction
// working with word of sz bytes
// Caution: sz must be 1, 2 or 4.
extern int ea_length(const Toperande* ea, int sz)
{
    switch (ea->kind) {
        case EaImmediate:
            return 1+sz;
        case EaDirectMemory:
            return 1+2;
        case EaInDirectMemory:
            return 1+2;
        case EaDirectRegister:
            return 1+0;
        case EaInDirectRegister:
            return 1+0;
    }
}

// writes into the memory pointed to by buf the binary code of
// the E.A. ea for an instruction working with word of sz bytes.
// Caution: sz must be 1, 2 or 4.
// Caution: the buf buffer must be large enough.
// returns the number of byte written into the buffer.
extern int ea_code(char* buf, const Toperande* ea, int sz, int last)
{
    int len=0;
    buf[len] = ea->kind+ea->pp + (last<<4);
	  switch (ea->kind) {
        case EaImmediate:
				len+=1;
				ds_writeBinary(buf+len, sz, cst_eval(ea->u.val));
				len+=sz;
		  		return len;	
        case EaDirectMemory:
				len+=1;
				ds_writeBinary(buf+len, 2, cst_eval(ea->u.val));
				len+=2;
            return len;
        case EaInDirectMemory:
				len+=1;
				ds_writeBinary(buf+len, 2, cst_eval(ea->u.val));
				len+=2;
            return len;
        case EaDirectRegister:
			   buf[len]+=ea->u.reg;
            return 1;
        case EaInDirectRegister:
			   buf[len]+=ea->u.reg;
            return 1;
    }   
	return len;
}

// prints on stdout the operande under human format
extern void ea_codeHum(const Toperande* ea, int sz, int last)
{
    int val;
    int len=0;
    switch (ea->kind) {
        case EaImmediate:
          val = cst_eval(ea->u.val);
          switch (sz) {
            case 1: printf(" $0x%02x", val&0xff); break;
            case 2: printf(" $0x%04x", val&0xffff); break;
            case 4: printf(" $0x%08x", val); break;
            default:printf(" $?sz=%d?",sz); break;
          }
          break;
        case EaDirectMemory:
          val = cst_eval(ea->u.val);
          printf(" @%04x", val&0xffff);
          break;
        case EaInDirectMemory:
          val = cst_eval(ea->u.val);
          printf(" [%04x]", val&0xffff);
          break;
        case EaDirectRegister:
          val = ea->u.reg;
          printf(" %%%02d", val&0xf);
          break;
        case EaInDirectRegister:
          val = ea->u.reg;
          printf(" %s[%%%02d]%s", ea->pp==0x40 ? "++" : "", val&0xf, ea->pp==0x20 ? "++" : "");
          break;
       /* default:
            fprintf(stderr,"%s: 0%2x invalid E.A kind in %s.\n",
                    ds_progname(),ea->kind,__func__);
            exit(1);*/
    }
	printf("\n");
}

/***************************************************************************/
/* effective address set                                                   */

extern ToperandeSet* eas_new()
{
    ToperandeSet* ret = (ToperandeSet*)malloc(sizeof(ToperandeSet));
    ret->nb=0;
    return ret;
}
extern void eas_delete(ToperandeSet*eas)
{
    int i;
    for (i=0;i<eas->nb;i++)
        ea_delete(eas->t[i]);
    free(eas);
}

// return: 1 for full otherwise 0.
extern int eas_isFull(const ToperandeSet*eas)
{ return eas->nb == EA_MaxNbOpe; }

extern ToperandeSet* eas_add(ToperandeSet*eas, Toperande* ea)
{
	 if(eas_isFull(eas)){fprintf(stderr,"error eas is full\n"); exit(1);}
	 eas->t[eas->nb] = ea ;
	 eas->nb+=1;
	 return eas;
}

extern int eas_length(const ToperandeSet* eas, int sz)
{
   int i; int size=0;
	for(i=0;i<eas->nb;i++){size+=ea_length(eas->t[i],sz);}
	return size;
}

extern int eas_code(char* buf, const ToperandeSet* eas, int sz)
{
    int i, len=0;
	 for(i=0;i<eas->nb;i++) 
	 {
		if(i==eas->nb-1) len+=ea_code(buf+len,eas->t[i], sz,1); //last ea
		else len+=ea_code(buf+len,eas->t[i], sz,0);
	 }
	 return len;
}

// prints on stdout the operande set under human format
extern void eas_codeHum(const ToperandeSet* eas, int sz)
{
	int i, len=0;
	 for(i=0;i<eas->nb;i++) 
	 {
		if(i==eas->nb-1) ea_codeHum(eas->t[i], sz, 1);
		else ea_codeHum(eas->t[i], sz, 0);
	 }
}

