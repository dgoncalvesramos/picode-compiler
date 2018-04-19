%{
#include <stdlib.h>
#include <string.h>
#include "asm.h"
#include "datastructure.h"
#include "const-tree.h"
#include "operande.h"
#include "asm.tab.h"
%}
%option noyywrap
%%

((add)|(sub)|(mul)|(in)|(out)|(mov)|(if))[bwl ] {
	if(strcmp(yytext,"add ")==0){yylval.opsz.op=INST_ADD;yylval.opsz.sz=0;}
	if(strcmp(yytext,"addb")==0){yylval.opsz.op=INST_ADD;yylval.opsz.sz=1;}
	if(strcmp(yytext,"addw")==0){yylval.opsz.op=INST_ADD;yylval.opsz.sz=2;}
	if(strcmp(yytext,"addl")==0){yylval.opsz.op=INST_ADD;yylval.opsz.sz=4;}
	if(strcmp(yytext,"sub ")==0){yylval.opsz.op=INST_SUB;yylval.opsz.sz=0;}
	if(strcmp(yytext,"subb")==0){yylval.opsz.op=INST_SUB;yylval.opsz.sz=1;}
	if(strcmp(yytext,"subw")==0){yylval.opsz.op=INST_SUB;yylval.opsz.sz=2;}
	if(strcmp(yytext,"subl")==0){yylval.opsz.op=INST_SUB;yylval.opsz.sz=4;}
	if(strcmp(yytext,"mul ")==0){yylval.opsz.op=INST_MUL;yylval.opsz.sz=0;}
	if(strcmp(yytext,"mulb")==0){yylval.opsz.op=INST_MUL;yylval.opsz.sz=1;}
	if(strcmp(yytext,"mulw")==0){yylval.opsz.op=INST_MUL;yylval.opsz.sz=2;}
	if(strcmp(yytext,"mull")==0){yylval.opsz.op=INST_MUL;yylval.opsz.sz=4;}
	if(strcmp(yytext,"in ")==0){yylval.opsz.op=INST_READ;yylval.opsz.sz=0;}
	if(strcmp(yytext,"inb")==0){yylval.opsz.op=INST_READ;yylval.opsz.sz=1;}
	if(strcmp(yytext,"inw")==0){yylval.opsz.op=INST_READ;yylval.opsz.sz=2;}
	if(strcmp(yytext,"inl")==0){yylval.opsz.op=INST_READ;yylval.opsz.sz=4;}
	if(strcmp(yytext,"out ")==0){yylval.opsz.op=INST_WRITE;yylval.opsz.sz=0;}
	if(strcmp(yytext,"outb")==0){yylval.opsz.op=INST_WRITE;yylval.opsz.sz=1;}
	if(strcmp(yytext,"outw")==0){yylval.opsz.op=INST_WRITE;yylval.opsz.sz=2;}
	if(strcmp(yytext,"outl")==0){yylval.opsz.op=INST_WRITE;yylval.opsz.sz=4;}
	if(strcmp(yytext,"mov ")==0){yylval.opsz.op=INST_MOV;yylval.opsz.sz=0;}
	if(strcmp(yytext,"movb")==0){yylval.opsz.op=INST_MOV;yylval.opsz.sz=1;}
	if(strcmp(yytext,"movw")==0){yylval.opsz.op=INST_MOV;yylval.opsz.sz=2;}
	if(strcmp(yytext,"movl")==0){yylval.opsz.op=INST_MOV;yylval.opsz.sz=4;}
	if(strcmp(yytext,"if ")==0){yylval.opsz.sz=0;return TK_IF;}
	if(strcmp(yytext,"ifb")==0){yylval.opsz.sz=1;return TK_IF;}
	if(strcmp(yytext,"ifw")==0){yylval.opsz.sz=2;return TK_IF;}
	if(strcmp(yytext,"ifl")==0){yylval.opsz.sz=4;return TK_IF;}
	return TK_OPSZ;
}

((jmp)|(call)|(ret)|(stop)|(then)|(else)) {
	if(strcmp(yytext,"jmp")==0){yylval.integer=INST_JMP; return TK_JUMP;}
	if(strcmp(yytext,"call")==0){yylval.integer=INST_EMP; return TK_CALL;}
	if(strcmp(yytext,"ret")==0){yylval.integer=INST_DEP; return TK_RET;}
	if(strcmp(yytext,"stop")==0){yylval.integer=INST_STOP; return TK_STOP;}
	if(strcmp(yytext,"then")==0){return TK_THEN;}
	if(strcmp(yytext,"else")==0){return TK_ELSE;}
}

(==) {yylval.integer=OPCMP_E;return TK_OC;}
(!=) {yylval.integer=OPCMP_NE;return TK_OC;}
(<) {yylval.integer=OPCMP_L;return TK_OC;}
(>) {yylval.integer=OPCMP_G;return TK_OC;}
(<=) {yylval.integer=OPCMP_LE;return TK_OC;}
(>=) {yylval.integer=OPCMP_GE;return TK_OC;}
"%"(([0-9])|(1[0-5])) {
   memmove(yytext, yytext+1, yyleng);
	yylval.integer=atoi(yytext);
	return TK_REG;
}

\.(org) {return TK_ORG;}
\.(long) {return TK_LONG;}
\.(word) {return TK_WORD;}
\.(byte) {return TK_BYTE;}
\.(string) {return TK_STR;}

"++" {return TK_PLUSPLUS;}
"<<" {return TK_SHIFT;}
[-+]+ {return *yytext;}

[a-zA-Z][a-z0-9A-Z_]*: {
	yytext[yyleng-1]=0;
	yylval.string = strdup(yytext);
	return TK_LABEL;
}
[a-zA-Z][a-z0-9A-Z_]* {
	yylval.string = strdup(yytext);
	return TK_CST_LAB;
}
\'[^\\']\' { 
	yylval.integer=yytext[1];
	return TK_CST_NUM;
}
[0-9]+ {
	yylval.integer = atoi(yytext);
	return TK_CST_NUM;
}

\"([^\\\"]|\\.)*\" {
	yylval.string=strdup(yytext);
	return TK_STRING;
}

[,\(\)\[\]@$*] {return *yytext;}
#.* {}
\n {yylineno++;}
[ \r\t]+ {}

. {return TK_ERROR;}
%%

void yyerror(const char* mess){
	fprintf(stderr,"IN FILE %s ERROR : %s (near %s) at line %d\n",ds_inputFilename(), mess, yytext, yylineno);
	exit(1);
}

