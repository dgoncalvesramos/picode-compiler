/***************************************************************************/
/* data structure                                                          */

#include <stdio.h>
#include <stdlib.h>

#include "asm.h"
#include "symbol.h"
#include "datastructure.h"

#define DS_LittleEndian  1
#define DS_BigEndian     2

typedef struct _Tdata {
    const char* progName;
    const char* inputFileName;
    int         endianess;      // see DS_XxxxEndian
    int         dump;           // 1:on 0:off
    int         passe2;         // 1:on 0:off
    Taddr       curAddr;
    TsymSet*     symSet;
    char*       binMem;         // BYTEMEMSIZE buffer
} Tdata;

static struct _Tdata ds ;

// initialize and free data structure
extern void  ds_init(const char* progName, const char* asmFileName)
{
    ds.progName      = progName;
    ds.inputFileName = asmFileName;
    ds.endianess     = DS_LittleEndian;
    ds.dump          = 0;
    ds.passe2        = 0;
    ds.curAddr       = 0;
    ds.symSet        = syms_new();
    ds.binMem        = (char*) calloc(BYTEMEMSIZE,1);
}

extern void  ds_free()
{
    fprintf(stderr,"FATAL:%s not yet implemented\n",__func__);
    exit(1);
}

// misc info. and opération
extern const char* ds_inputFilename()   { return ds.inputFileName; }
extern const char* ds_progname()        { return ds.progName; }
extern void        ds_setLittleEndian() { ds.endianess=DS_LittleEndian; }
extern void        ds_setBigEndian()    { ds.endianess=DS_BigEndian; }
extern void        ds_setDump()         { ds.dump=1; }
extern int         ds_dumpIsOn()        { return ds.dump; }
extern void        ds_setPasse2()       { ds.curAddr=0; ds.passe2=1; }
extern int         ds_passe2IsOn()      { return ds.passe2; }
extern int         ds_writeBinary(char*p,int sz,unsigned int value)
{
    unsigned int be_val = value; //htonl(value);
    if ( ds.endianess==DS_LittleEndian) {
        switch (sz) {
            case 1: p[0] = (be_val >> 0) & 0xFF; break;
            case 2: p[0] = (be_val >> 0) & 0xFF;
                    p[1] = (be_val >> 8) & 0xFF; break;
            case 3: p[0] = (be_val >> 0) & 0xFF;
                    p[1] = (be_val >> 8) & 0xFF;
                    p[2] = (be_val >>16) & 0xFF; break;
            case 4: p[0] = (be_val >> 0) & 0xFF;
                    p[1] = (be_val >> 8) & 0xFF;
                    p[2] = (be_val >>16) & 0xFF;
                    p[3] = (be_val >>24) & 0xFF; break;
        }
    } else {
        switch (sz) {
            case 1: p[0] = (be_val >> 0) & 0xFF; break;
            case 2: p[0] = (be_val >> 8) & 0xFF;
                    p[1] = (be_val >> 0) & 0xFF; break;
            case 3: p[0] = (be_val >>16) & 0xFF;
                    p[1] = (be_val >> 8) & 0xFF;
                    p[2] = (be_val >> 0) & 0xFF; break;
            case 4: p[0] = (be_val >>24) & 0xFF;
                    p[1] = (be_val >>16) & 0xFF;
                    p[2] = (be_val >> 8) & 0xFF;
                    p[3] = (be_val >> 0) & 0xFF; break;
        }
    }
    return sz;
}

// current address handler
extern Taddr ds_getCurAddr() { return ds.curAddr; }
extern Taddr ds_incCurAddr(int inc) { ds.curAddr+=inc; return ds.curAddr; }
extern char* ds_getMem()     { return ds.binMem; }
extern char* ds_getCurMem()  { return ds.binMem+ds.curAddr; }

// Wrapper to the function of label.h
extern void  ds_symAdd(char*ln, Taddr a) {syms_add(&ds.symSet,ln,a);}
extern Taddr ds_symGetAddr(char*ln) {return syms_getAddr(ds.symSet,ln);}
extern void  ds_symAddSymIfNotExist(char*ln)
{   
    if ( syms_search(ds.symSet,ln) ) return;
    syms_add(&ds.symSet,ln,ADDR_UNDEF);
}
extern void  ds_symWalk()
{
	syms_walk(ds.symSet); 
}

void ds_dumpWriteBin(int len)
{
    int i,j,nbc0,nbc;
    nbc=nbc0=printf("%04x :",ds_getCurAddr()&0xFFFF);
    for (i=0 ; i<len ; i++) {
        if ( (nbc+3)>=30 ) {
            printf("\n");
            for (nbc=0 ; nbc<nbc0 ; nbc++) printf(" ");
        }
        nbc += printf(" %02x",ds_getCurMem()[i]&0xff);
    }
    for ( ; nbc<30 ; nbc++)
        printf(" ");
    printf(": ");
}
