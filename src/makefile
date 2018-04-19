# Declaration de variables personnalisees
CC=gcc
LEX=flex
YACC=bison
EXEC=aspico
CFLAG=-c -g
FLIBRARY=-lfl

# Règle regroupant dans sa dependance l'executable a produire
all: $(EXEC) 

$(EXEC): main.o datastructure.o operande.o const-tree.o symbol.o asm.tab.o lex.yy.o yacc-semantic.o 
	$(CC) -o $(EXEC) $^ $(FLIBRARY)

main.o: main.c
	$(CC) -o $@ $(CFLAG) $<

datastructure.o: datastructure.c
	$(CC) -o $@ $(CFLAG) $<

yacc-semantic.o: yacc-semantic.c
	$(CC) -o $@ $(CFLAG) $<

symbol.o: symbol.c
	$(CC) -o $@ $(CFLAG) $<

const-tree.o: const-tree.c
	$(CC) -o $@ $(CFLAG) $<

operande.o: operande.c
	$(CC) -o $@ $(CFLAG) $<

lex.yy.o: lex.yy.c
	$(CC) -o $@ $(CFLAG) $<

lex.yy.c: asm.l
	$(LEX) $<

asm.tab.o: asm.tab.c 
	$(CC) -o $@ $(CFLAG) $<

asm.tab.c: asm.y
	$(YACC) -d $<

automate: asm.y
	$(YACC) -g  asm.y && dot -Tpdf asm.dot -o asm_automate.pdf

output: asm.y
	$(YACC) -v asm.y

# Cible .PHONY
.PHONY: clean

# Permet de supprimer tout ce qui peut être regenere et permet une reconstruction complète du projet
clean: 
	rm -rf asm.tab.* lex.yy.c asm.dot asm.output asm_automate.pdf *.o $(EXEC)

