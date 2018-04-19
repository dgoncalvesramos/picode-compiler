%{
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "operande.h"
#include "const-tree.h"
#include "yacc-semantic.h"
%}

/* Definition types */
%union {
	char* string;
	int integer;
	char character;
	Toperande* operande;
	Tconst* expr; 
	ToperandeSet* suiteOp;
	struct {
		int op,sz;
	}opsz;
}

%token<string> TK_LABEL
%token<integer> TK_CST_NUM
%token<string> TK_CST_LAB 
%token<opsz> TK_OPSZ
%token<integer> TK_REG
%token<integer> TK_JUMP
%token<integer> TK_CALL
%token<integer> TK_RET
%token<integer> TK_STOP
%token<integer> TK_OC
%token<opsz> TK_IF 
%token TK_ELSE
%token TK_THEN
%token TK_ORG 
%token TK_LONG
%token TK_WORD 
%token TK_BYTE 
%token TK_STR 
%token TK_PLUSPLUS 
%token TK_SHIFT 
%token<string> TK_STRING
%token TK_ERROR 
%type<expr> expr
%type<expr> ais_optionnel 
%type<operande> operande
%type<suiteOp> suite_operande_src 
%left '-' '+'
%left '*'
%left TK_SHIFT

%start prog	
%%

prog : suite_elements ;

suite_elements 
	: suite_elements element 
	|
;

element 
	: instruction
	| TK_LABEL {if(!ds_passe2IsOn())ds_symAdd($1,ds_getCurAddr());}
   | primitive 
;

instruction 
	/*Instuction avec opérandes*/
	: TK_OPSZ operande suite_operande_src ais_optionnel{startYaccInstruction($1.op, $1.sz,$2,$3,$4);}
	| TK_IF operande TK_OC operande TK_THEN TK_CST_LAB {startYaccIf($1.sz,$2,$3,$4,$6,NULL);}
	| TK_IF operande TK_OC operande TK_THEN TK_CST_LAB TK_ELSE TK_CST_LAB{startYaccIf($1.sz,$2,$3,$4,$6,$8);} //+2o of cst-false branch
 
	/*Instruction sans opérandes*/  
	| TK_JUMP expr {startYaccJmp($1,$2);}
   | TK_CALL expr ais_optionnel {startYaccCall($1,$2,$3);}
	| TK_RET {startYaccRet($1);}
	| TK_STOP{startYaccStop($1);} 
		 
;

ais_optionnel 
	: 	'@'expr{$$=$2;} 
   	| {$$=0;}
	   
;

suite_operande_src 
	: suite_operande_src ',' operande {$$=eas_add($1,$3);}
	| {$$=eas_new();}
;

operande 
	: '$'expr{$$=ea_newImmediat($2);}
	|  expr{$$=ea_newDirectMemory($1);}
	| '['expr']'{$$=ea_newInDirectMemory($2);}
	|  TK_REG {$$=ea_newDirectRegister($1);}
	| '['TK_REG']' {$$=ea_newInDirectRegister($2);}
	| TK_PLUSPLUS'['TK_REG']' {$$=ea_newInDirectRegisterPreInc($3);}
	| '['TK_REG']'TK_PLUSPLUS {$$=ea_newInDirectRegisterPostInc($2);}
;

expr 
	:TK_CST_LAB {$$=cst_newIdent($1);}
	|TK_CST_NUM {$$=cst_newNumber($1);}
	|expr'+'expr{$$=cst_newBinOp($1,CST_PLUS,$3);}
	|expr'-'expr{$$=cst_newBinOp($1,CST_MINUS,$3);}
	|expr'*'expr{$$=cst_newBinOp($1,CST_TIME,$3);}
	|expr TK_SHIFT expr{$$=cst_newBinOp($1,CST_LSH,$3);}
	|'('expr')' {$$=$2;}
   |'+'expr{$$=$2;}
	|'-'expr{$$=cst_newBinOp(cst_newNumber(0), CST_MINUS,$2);}
;

primitive 
	: TK_ORG expr 	{startYaccOrgExpr($2);}
	| TK_LONG expr {startYaccLong($2);}
   | TK_WORD expr {startYaccWord($2);}
   | TK_BYTE expr {startYaccByte($2);}
   | TK_STR TK_STRING{startYaccStr($2);}
%%
