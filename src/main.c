#include <stdio.h>
#include <stdlib.h>
#include "datastructure.h"

extern FILE *yyin;
int flagSymTab=0;
int flagDump=0;

// Option -t
void print_table()
{
	/*Print other symbs of symset*/
	ds_symWalk();
}

//option -h
void print_help()
{
	printf("HELP : Usage: ./aspico <input_file> [-t] [-d] [-o] <output_file>\n");
	printf("-t : Afficher la table des symboles\n");
	printf("-d : Générer un fichier dump\n");
	printf("-o <output_files> Renomme le fichier exécutable (nom par défaut = picode)\n");
}

// Generer le fichier PICODE
void generate_exe_name(char* exe_name)
{ 
	FILE* exe = NULL ;
	int i;
	if((exe=fopen(exe_name,"w"))==NULL){
		fprintf(stderr, "Erreur ouverture fichier impossible\n");
		exit(1);
	}
	fwrite(ds_getMem(),1,BYTEMEMSIZE,exe);
	fclose(exe);
}
	
void parse_args(int argc, char* argv[], char** exe_name)
{
	 int i;
	 int find_option_o = 0;

	if(argc<2)
	{
		fprintf(stderr, "Usage: ./aspico <input_file> [-t] [-d] [-o] <output_file>\n");
		exit(1);
	}

	/*SANS ARGUMENT*/
	if((yyin=fopen(argv[1],"r"))==NULL) 
	{
		/*HELP OPTION*/
		if(strcmp(argv[1],"-h")==0)
		{
			print_help();
			exit(0);
		}

		else /*Erreur lecture fichier*/
		{
			fprintf(stderr, "Usage: ./aspico <input_file> [-t] [-d] [-o] <output_file>\n");
			exit(1);
		}
	}
	
	/*VERIFICATION ARGUMENT*/
	for(i=2;i<argc;i++)
	{
		if(strcmp(argv[i],"-t")==0 && find_option_o==0) 
		{
			flagSymTab=1;
		}
		else if(strcmp(argv[i],"-d")==0 && find_option_o==0)
		{
			flagDump=1;
		}

		else if(strcmp(argv[i],"-o")==0 && find_option_o==0)
		{
			/*VERICATION FICHIER OUTPUT*/
			if(i == argc-1) 
			{
				fprintf(stderr, "Usage: ./aspico <input_file> [-t] [-d] [-o] <output_file>\n");
				exit(1);
			}
			else
			{
				/*VERIFICATION NOM FICHIER OUTPUT RENSEIGNEE*/
				if(strcmp(argv[i+1]," ")!=0)
				{
					*exe_name = argv[i+1] ;
					find_option_o = 1;		
				}
					
				else 
				{
					fprintf(stderr, "Usage: ./aspico <input_file> [-t] [-d] [-o] <output_file>\n");
					exit(1);
				}
			}
		}
		else if(find_option_o==1) 
		{
			/* DO NOTHING */
			find_option_o = 0 ; 
		}

		else
		{
			fprintf(stderr, "Usage: ./aspico <input_file> [-t] [-d] [-o] <output_file>\n");
			exit(1);
		}
	}
}

int main (int argc, char* argv[])
{
	char* exe_name = "picode"; //nom par défaut
	char* asmFile = argv[1];
	parse_args(argc, argv, &exe_name);
	ds_init(exe_name, asmFile);
	if(yyparse()==0){
		if(flagDump) ds_setDump();
		if(flagSymTab) print_table(); //print symbol table
		ds_setPasse2();//set passe 2 on DS
		rewind(yyin);
	   yyparse(); //second passe
		generate_exe_name(exe_name); //generate exe file
	}
	return 0;
}

