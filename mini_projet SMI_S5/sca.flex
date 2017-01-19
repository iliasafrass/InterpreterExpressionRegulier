%{
#include<stdio.h>
#include "par.tab.h"
int taille=0;
%}

%%
"Help"		{return(Help);}
"Is" 		{return(Is);}
"Nullable"	{return(Nullable);}
"In"		{return(In);}
"Empty"		{return(Empty);}
"Finite"	{return(Finite);}
"Infinite"	{return(Infinite);}
"Symbol"	{return(Symbol);}
"Single" 	{return(Single);} 
"Epsilon"	{return(Epsilon);}
"Print"		{return(Print);}
"AST"		{return(AST);}
"Quit"		{return(FIN);}
"a" |
"b" |
"1" |
"0" |		
"(" |
")" |
"|" |
"*" |
"?" |
"<" |
\n  |
">" 		return *yytext;
[\t ] 		;
.		{printf("\n## Lexical error ##");yyterminate();}	

%%