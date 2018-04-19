/***************************************************************************/
/* semantic analyse                                                        */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "yacc-semantic.h"
#include "operande.h"
/* Verify buff size
 * Incrémente la taille du buffer et l'évalue
   si la taille dépasse 2^15o (BYTEMEMSIZE) erreur
*/

static void verifyBuffSize(int inc)
{
	if(ds_incCurAddr(inc)>BYTEMEMSIZE)
		yyerror("buffer out of memory");
}


extern void startYaccInstruction(TinstOpCode code, int sz,Toperande* opDest,ToperandeSet* listOpSrc, Tconst* cstAis)
{
	/*calculate size of inst*/
	int instSize=0;
	instSize+=3; //OP+sz+addr next inst
   instSize+=ea_length(opDest,sz); //opDest
   instSize+=eas_length(listOpSrc,sz); //liste opsrc size
   Taddr nextAddr = ds_getCurAddr()+instSize;
	if(ds_passe2IsOn())
	{
		int i,lenWriteDest,lenWriteSrc;
		TinstOpCode codeOp = code ;
      if(sz==1) codeOp+=1;
		else if(sz==2) codeOp+=2;
	   else if(sz==4) codeOp+=3;
      ds_writeBinary(ds_getCurMem(),1,codeOp);
		ds_incCurAddr(1);
		Taddr addr = cstAis==0?nextAddr:cst_eval(cstAis);
		ds_writeBinary(ds_getCurMem(),2,addr);
		ds_incCurAddr(2);
		if(listOpSrc->nb==0) ds_incCurAddr(lenWriteDest=ea_code(ds_getCurMem(),opDest,sz,1)); //write ea for opdest when last
		else ds_incCurAddr(lenWriteDest=ea_code(ds_getCurMem(), opDest,sz,0)); //write ea for opdest when not last
		ds_incCurAddr(lenWriteSrc=eas_code(ds_getCurMem(),listOpSrc,sz)); //write ea for eas set

		/*******************************************DUMP******************************************/
		if(ds_dumpIsOn()){
			printf("************************************\n\n");
			printf("INSTRUCTION : ");
			switch(code){
				case INST_ADD :
					printf("ADD\n");
					break;
				case INST_SUB :
					printf("SUB\n");
					break;
				case INST_MUL :
					printf("MUL\n");
					break;
				case INST_MOV :
					printf("MOV\n");
					break;
				case INST_READ :
					printf("IN\n");
					break;
				case INST_WRITE :
					printf("OUT\n");
					break;
			}
			printf("SZ= %d\n",sz);
			printf("INST SIZE= %d\n", instSize);
			printf("ADDR NEXT INST = %d\n", addr);
			printf("Opérande destination :\n");
			if(listOpSrc->nb==0) ea_codeHum(opDest,sz,1);
			else ea_codeHum(opDest,sz,0);
			if(listOpSrc->nb>0)printf("Opérande(s) source(s) :\n");
			eas_codeHum(listOpSrc,sz);
			ds_incCurAddr(-instSize);
			printf("Binary dump of instruction :\n");
			ds_dumpWriteBin(instSize);
			printf("\n\n");
			printf("************************************\n\n");
			ds_incCurAddr(instSize);
			/***************************************************************************************/ 
		}
	}

	else
	{
		if(opDest->kind == EaImmediate && code!=INST_WRITE) //if inst_write ok
			yyerror("Operand destination kind cannot be immediate");
		switch(code)
		{
			case INST_ADD :
				if(listOpSrc->nb < 2) yyerror("At least 2 sources operands for add inst");
				break;
			case INST_SUB :
				if(listOpSrc->nb < 2) yyerror("At least 2 sources operands for sub inst");
				break;
			case INST_READ:
				if(listOpSrc->nb != 0) yyerror("No sources operands allowed for instruction read");
				break;
			case INST_WRITE:
				if(listOpSrc->nb != 0) yyerror("No sources operands allowed for instruction write");
				break;
			case INST_MOV:
				if(listOpSrc->nb != 1) yyerror("Only one source operand allowed for instruction mov");
		}

		verifyBuffSize(instSize); //inc buffer
	}

	eas_delete(listOpSrc); //clear eas
	ea_delete(opDest); //clear opDest
	if(cstAis!=NULL) cst_delete(cstAis); //clear cstAis
}


extern void startYaccIf(int sz, Toperande* opDest, Topcmp opcmp, Toperande* opSrc, char* branch_addr_true, char* branch_addr_false)
{
	/*calculate size of inst*/
	int instSize=0;int i;Taddr nextAddr;
	instSize+=3; //OPSZ + addr next inst
	instSize+=ea_length(opDest,sz); //opDest
	instSize+=ea_length(opSrc,sz); //opSrc
	instSize+=2;//N2
	nextAddr=ds_getCurAddr()+instSize;
	if(ds_passe2IsOn()){
    	Toperande* new_OpDest=opDest;
		Toperande* new_OpSrc=opSrc;
		char* new_branch_addr_true=branch_addr_true;
		char* new_branch_addr_false=branch_addr_false;
		TinstOpCode codeOp;
		switch(opcmp){
			case OPCMP_G :
				new_OpDest=opSrc;
				new_OpSrc=opDest;
				codeOp=INST_JL;
				break;
			case OPCMP_GE :
				new_OpDest=opSrc;
				new_OpSrc=opDest;
				codeOp=INST_JLE;
				break;
			case OPCMP_NE :
				new_branch_addr_true=branch_addr_false;
				new_branch_addr_false=branch_addr_true;
				codeOp=INST_JE;
				break;
			case OPCMP_E :
				codeOp=INST_JE;
				break;
			case OPCMP_L :
				codeOp=INST_JL;
				break;
			case OPCMP_LE :
				codeOp=INST_JLE;
				break;
		}		
      if(sz==1) codeOp+=1;
		else if(sz==2) codeOp+=2;
	   else if(sz==4) codeOp+=3;
      ds_writeBinary(ds_getCurMem(),1,codeOp); //write opCode+Sz
		ds_incCurAddr(1);
		if(new_branch_addr_false==NULL) ds_writeBinary(ds_getCurMem(),2,nextAddr);//N=addr next inst
		else ds_writeBinary(ds_getCurMem(),2,ds_symGetAddr(new_branch_addr_false));//N=addr of symb branch false
		ds_incCurAddr(2);
	   ds_incCurAddr(ea_code(ds_getCurMem(),new_OpDest,sz,0)); //write opDest
		ds_incCurAddr(ea_code(ds_getCurMem(),new_OpSrc,sz,1)); //write opSrc
		if(new_branch_addr_true==NULL) ds_writeBinary(ds_getCurMem(),2,nextAddr); //N2=addr next inst
		else ds_writeBinary(ds_getCurMem(),2,ds_symGetAddr(new_branch_addr_true)); //N2=addr true
		ds_incCurAddr(2);

		/*******************************************DUMP******************************************/
		if(ds_dumpIsOn()){
			printf("************************************\n\n");
			printf("INSTRUCTION : \n");
			if(sz==1) codeOp-=1;
			else if(sz==2) codeOp-=2;
	   	else if(sz==4) codeOp-=3;
			switch(codeOp){
				case INST_JL :
					printf("JL\n");
					break;
				case INST_JE :
					printf("JE\n");
					break;
				case INST_JLE :
					printf("JLE\n");
					break;
			}
			printf("SZ= %d\n",sz);
			printf("INST SIZE= %d\n", instSize);
			if(new_branch_addr_true==NULL) printf("ADDR NEXT INST IF TRUE = %d\n", nextAddr);
			else printf("ADDR NEXT INST IF TRUE = %d\n", ds_symGetAddr(new_branch_addr_true));
			if(new_branch_addr_false==NULL) printf("ADDR NEXT INST IF FALSE = %d\n", nextAddr);
			else printf("ADDR NEXT INST IF FALSE = %d\n", ds_symGetAddr(new_branch_addr_false));
			printf("Opérande destination :\n");
			ea_codeHum(opDest,sz,0);
			printf("Opérande source :\n");
			ea_codeHum(opSrc,sz,1);
			ds_incCurAddr(-instSize);
			printf("Binary dump of instruction :\n");
			ds_dumpWriteBin(instSize);
			printf("\n\n");
			printf("************************************\n\n");
			ds_incCurAddr(instSize);
		}
		/******************************************************************************************/
	}
	else{
		verifyBuffSize(instSize);
		ea_delete(opDest);
		ea_delete(opSrc);
	}
}


/*Start instuction without source operands 
********************************************************************/
extern void startYaccJmp(TinstOpCode code, Tconst* cst_branch)
{
	if(ds_passe2IsOn()){
		 ds_writeBinary(ds_getCurMem(),1,code);
		 ds_incCurAddr(1);
		 ds_writeBinary(ds_getCurMem(),2,cst_eval(cst_branch));
		 ds_incCurAddr(2);
		/*******************************************DUMP******************************************/
		if(ds_dumpIsOn()){
			printf("************************************\n\n");
			printf("INSTRUCTION : \n");
			printf("JMP\n");
			printf("INST SIZE= %d\n", 3);
			printf("ADDR NEXT INST = %d\n", cst_eval(cst_branch));
			ds_incCurAddr(-3);
			printf("Binary dump of instruction :\n");
			ds_dumpWriteBin(3);
			printf("\n\n");
			printf("************************************\n\n");
			ds_incCurAddr(3);
		}
		/******************************************************************************************/
	}
	else{
		if(cst_branch == NULL) yyerror("Jmp inst must have a cst-branch");
		verifyBuffSize(3);
	}
}

extern void startYaccCall(TinstOpCode code, Tconst* cst_branch, Tconst* AisOptionnel)
{
	if(ds_passe2IsOn()){
		 Taddr nextAddr = ds_getCurAddr()+5;
		 ds_writeBinary(ds_getCurMem(),1,code);
		 ds_incCurAddr(1);
		 Taddr addr1 = AisOptionnel==0?nextAddr:cst_eval(AisOptionnel);
		 ds_writeBinary(ds_getCurMem(),2,addr1);
		 ds_incCurAddr(2);
		 Taddr addr2 = cst_eval(cst_branch);
		 ds_writeBinary(ds_getCurMem(),2,addr2);
		 ds_incCurAddr(2);
		 /*******************************************DUMP******************************************/
		 if(ds_dumpIsOn()){
			printf("************************************\n\n");
			printf("INSTRUCTION : \n");
			printf("CALL\n");
			printf("INST SIZE= %d\n", 5);
			printf("ADDR EMPILE= %d\n",addr1);
			printf("ADDR NEXT INST = %d\n",addr2);
			ds_incCurAddr(-5);
			printf("Binary dump of instruction :\n");
			ds_dumpWriteBin(5);
			printf("\n\n");
			printf("************************************\n\n");
			ds_incCurAddr(5);
		 }
		/******************************************************************************************/
	}
	else{
		if(cst_branch==NULL) yyerror("Call inst must have a cst-branch");
		else verifyBuffSize(5);
	}
}

extern void startYaccRet(TinstOpCode code)
{
	if(ds_passe2IsOn()){
		ds_writeBinary(ds_getCurMem(),1,code);
		ds_incCurAddr(1);
		/*******************************************DUMP******************************************/
		 if(ds_dumpIsOn()){
			printf("************************************\n\n");
			printf("INSTRUCTION : \n");
			printf("RET\n");
			printf("INST SIZE= %d\n", 1);
			ds_incCurAddr(-1);
			printf("Binary dump of instruction :\n");
			ds_dumpWriteBin(1);
			printf("\n\n");
			printf("************************************\n\n");
			ds_incCurAddr(1);
		 }
		/******************************************************************************************/
	}
	else{
		verifyBuffSize(1);
	}
}

extern void startYaccStop(TinstOpCode code)
{
	if(ds_passe2IsOn()){
		ds_writeBinary(ds_getCurMem(),1,code);
		ds_incCurAddr(1);
		/*******************************************DUMP******************************************/
		 if(ds_dumpIsOn()){
			printf("************************************\n\n");
			printf("INSTRUCTION : \n");
			printf("STOP\n");
			printf("INST SIZE= %d\n", 1);
			ds_incCurAddr(-1);
			printf("Binary dump of instruction :\n");
			ds_dumpWriteBin(1);
			printf("\n\n");
			printf("************************************\n\n");
			ds_incCurAddr(1);
		 }
		/******************************************************************************************/
	}
	else{
		verifyBuffSize(1);
	}
}
/*******************************************************************/

/*Start Yacc primitives
********************************************************************/
extern void startYaccOrgExpr(Tconst* cst)
{ 
	if(ds_passe2IsOn()){
		if(cst_withIdent(cst)>0 && ds_symGetAddr(cst->u.ident)!=ADDR_UNDEF) yyerror("Org primitives cannot be a symbole");
	}
	else{
		if(cst==NULL) yyerror("Cst cannot be null for org primitive");	
	}
	verifyBuffSize(cst_eval(cst));
}
extern void startYaccLong(Tconst* cst)
{
	if(cst==NULL) yyerror("Cst cannot be null for long primitive");	
   verifyBuffSize(4);
}

extern void startYaccWord(Tconst* cst)
{
	if(cst==NULL) yyerror("Cst cannot be null for word primitive");	
	verifyBuffSize(2);
}

extern void startYaccByte(Tconst* cst)
{
	if(cst==NULL) yyerror("Cst cannot be null for byte primitive");	
	verifyBuffSize(1);
}

extern void startYaccStr(char* str)
{
	verifyBuffSize(strlen(str)+1);
}
/********************************************************************/
