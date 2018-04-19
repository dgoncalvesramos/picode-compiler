/***************************************************************************/
/* data structure                                                          */
#ifndef FILE_DATASTRUCTURE_H
#define FILE_DATASTRUCTURE_H

#include "symbol.h"

/***************************************************************************/
// initialize and free data structure
extern void  ds_init(const char* progName, const char* asmFileName);
extern void  ds_free();

// misc info. and operations
extern const char* ds_inputFilename();
extern const char* ds_progname();
extern void        ds_setLittleEndian();
extern void        ds_setBigEndian();
extern void        ds_setPasse2(); // + reset curAddr
extern int         ds_passe2IsOn();
extern void        ds_setDump();
extern int         ds_dumpIsOn();

/***************************************************************************/
// returns the current address.
extern Taddr ds_getCurAddr();
// increments by inc the current address and then returns it.
extern Taddr ds_incCurAddr(int inc);
// returns a pointer to the PICO memory buffer.
extern char* ds_getMem();
// returns a pointer to the current position into the PICO memory
// buffer: ds_getMem()+ds_getCurAddr()
extern char* ds_getCurMem();

/***************************************************************************/
// Wrapper to the function of label.h
extern void  ds_symAdd(char*sn, Taddr a);
extern Taddr ds_symGetAddr(char*sn);
extern void  ds_symAddSymIfNotExist(char*sn);
extern void  ds_symWalk();

/***************************************************************************/
// write the sz low bytes of value into the memory pointed to by p
// according to the endianess. sz must be in { 0, 1, 2, 3, 4}.
// Returns sz.
extern int   ds_writeBinary(char*p,int sz,unsigned int value);

// print into the stdout stream the line
//     addr : b0 b1 b2 b3 ... b(n-1) :
// where addr is ds_getCurMem(), and b(i) are the values of ds_getCurMem()[i]
extern  void ds_dumpWriteBin(int n);
#define      ds_dumpWriteHum printf

#endif // FILE_DATASTRUCTURE_H
