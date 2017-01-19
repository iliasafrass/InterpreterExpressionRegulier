%{
#include<stdio.h>
#include<stdlib.h>
#include "ast.h"
#include<string.h>
%}

%union{
  struct ast *a;
  char * chaine;
}

%start text
%token <chaine> Is Nullable In Empty Finite Infinite Symbol Single Epsilon Print AST FIN Help
%type <a> exp T F H
%type <chaine> word str 
 


%%

text :
     | text line 
     ;

line : '\n' {printf("veuillez saisir du texte \n");}
     | exp '\n' 			{printf("valide, taille = %d\n",size($1));
					 liberer_ast($1);}
     | Is exp Nullable '?' '\n' 	{if(contient_epsilon($2))
					  puts("Yes");
					 else
					  puts("No");}
     | Is '<' word '>' In exp '?' '\n'	{if(appartient($3,$6))
					  puts("Yes");
					 else
					  puts("No");}
     | Is exp Empty '?'	'\n'		{if(est_vide($2))
					  puts("Yes");
					 else
					  puts("No");}
     | Is exp Finite '?' '\n'		{if(est_fini($2))
					  puts("Yes");
					 else
					  puts("No");}
     | Is exp Infinite '?' '\n'		{if(est_infini($2))
					  puts("Yes");
					 else
					  puts("No");}
     | Is exp Symbol '?' '\n'		{if(est_symbole($2))
					  puts("Yes");
					 else
					  puts("No");}
     | Is exp Single '?' '\n'		{if(est_single($2))
					  puts("Yes");
					 else
					  puts("No");}
     | Is exp Epsilon '?' '\n'		{if(est_epsilon($2))
					  puts("Yes");
					 else
					  puts("No");}
     | Print AST exp '\n'		{FILE *fichier=fopen("ast.xml","r");
					 char car;
					 imprimer_ast($3);
					 do{car=fgetc(fichier);
      					    printf("%c",car);
     					    }while(car!=EOF);
					 liberer_ast($3);
					}
     | FIN				{printf("\nmerci ! et a bientot\n");YYACCEPT;}
     | Help				{puts("PROJECT");
					 puts("   Regular Expression Calculator over the alphabet A={a,b}");
					 puts("COMMANDES");
					 puts("   Help");
					 puts("     Affiche l'aide sur l'interpreteur ");
					 puts("   Quit");
					 puts("     Quitte l'interpreteur");
					 puts("   exp");
					 puts("     Verifier si exp est une exp valide");
					 puts("     Affiche egalement sa taille");
					 puts("   Is exp Nullable ?");
					 puts("     Teste si L(exp) contient le mot vide (epsilon)"); 	
					 puts("   Is <word> In exp ?");	
					 puts("     Teste si L(exp) contient le mot donne (word)");
					 puts("   Is exp Empty ?");
					 puts("     Teste si L(exp) est vide"); 
					 puts("   Is exp Finite ?");
					 puts("     Teste si L(exp) est fini");
					 puts("   Is exp Infinite ?");
					 puts("     Teste si L(exp) est infini");
					 puts("   Is exp Symbol ?");
					 puts("     Teste si L(exp) est {a} ou bien {b} (un symbole)");
					 puts("   Is exp Single ?");
					 puts("     Teste si L(exp) est un singleton (reduit a un seul mot)");
					 puts("   Is exp Epsilon ?");
					 puts("     Teste si L(exp) est {epsilon} ");
					 puts("   Print AST exp");
					 puts("     Cree un fichier XML nomme \"ast.xml\" representant l'AST de exp");
					 puts("     Affiche egalement le contenu de ce fichier");
			      		}
     ;

exp :  exp  '|' T   { $$=creer_union($1,$3);}
    |  T
    ;

T : T F  	    {$$=creer_concat($1,$2);}
  | F
  ;

F : F '*'           {$$=creer_etoile($1);}
  | H
  ;

H : 'a' 		{$$=creer_a();}
  | 'b' 		{$$=creer_b();}
  | '1' 		{$$=creer_epsilon();}
  | '0' 		{$$=creer_vide();}
  | '(' exp ')' 	{$$=copier_ast($2);}
  ;

word : word str 	{$$=strcat($1,$2);}
     | str
     ;

str : 'a' { $$ = malloc(2*sizeof(char)); strcpy($$,"a"); }
    | 'b' { $$ = malloc(2*sizeof(char)); strcpy($$,"b"); }
    | '1' { $$ = malloc(2*sizeof(char)); strcpy($$,"1"); }
    ;
%%
main(){
puts("########################################################");
puts("#     this interpreter was written by Ilias Afrass     #");
puts("#                  CNE:1210874234                      #");
puts("########################################################");
puts("\n$$ -Taper la commande Help pour lire l'aide");
puts("\n$$ -Taper une expression reguliere");
puts("\n$$ -Taper un Question Is...");
puts("\n$$ -Taper la commande Quit pour quitter l'application\n\n");
yyparse();
}
