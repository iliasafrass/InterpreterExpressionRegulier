#include<stdio.h>
#include<stdlib.h>
#include<string.h>
#include "ast.h"

char *element[100];
int i=0;
int s=0;

ast* creer_a(){
   ast* a=(ast *)malloc(sizeof(ast));
   a->type=A;
   a->fd=NULL;
   a->fg=NULL;
   return a;
}

ast* creer_b(){
   ast* b=(ast *)malloc(sizeof(ast));
   b->type=B;
   b->fd=NULL;
   b->fg=NULL;
   return b;
}

ast* creer_epsilon(){
   ast* e=(ast *)malloc(sizeof(ast));
   e->type=EPSILON;
   e->fd=NULL;
   e->fg=NULL;
   return e;
}

ast* creer_vide(){
   ast* v=(ast *)malloc(sizeof(ast));
   v->type=VIDE;
   v->fd=NULL;
   v->fg=NULL;
   return v;
}

ast* creer_union(ast* e1,ast* e2){
   ast* u=(ast *)malloc(sizeof(ast));
   u->type=UNION;
   u->fg=e1;
   u->fd=e2;
   return u;
}

ast* creer_concat(ast* e1,ast* e2){
   ast* c=(ast *)malloc(sizeof(ast));
   c->type=CONCAT;
   c->fg=e1;
   c->fd=e2;
   return c;
}

ast* creer_etoile(ast* e){
   ast *et=(ast *)malloc(sizeof(ast));
   et->type=ETOILE;
   et->fd=NULL;
   et->fg=e;
   return et;
}

ast* copier_ast(ast* e){
   ast* c=(ast *)malloc(sizeof(ast));
   c->type=e->type;
   c->fd=e->fd;
   c->fg=e->fg;
   return c;
}

void liberer_ast(ast* e){
   s=0;
   switch(e->type){
	case A: 
	
	case B: 	

	case EPSILON: 	

	case VIDE: 	free(e);break;

	case ETOILE:	liberer_ast(e->fg);
			free(e);
			break;
	case UNION:
	case CONCAT:	liberer_ast(e->fg);
			liberer_ast(e->fd);
			free(e);
			break;	
}	
   e=NULL;	
}

void remplir_elements_ast(ast *e){
   if(e!=NULL){
	switch(e->type){
		case A: 	s++;
				element[i]=(char *)malloc(5*sizeof(char));
				strcpy(element[i],"<a/>");
				break;

		case B: 	s++;
				element[i]=(char *)malloc(5*sizeof(char));
				strcpy(element[i],"<b/>");
				break;
		
		case EPSILON:   element[i]=(char *)malloc(11*sizeof(char));
			        strcpy(element[i],"<epsilon/>");
			        break;
		
		case VIDE:      element[i]=(char *)malloc(8*sizeof(char));
				strcpy(element[i],"<vide/>");
				break;
			   
		case ETOILE:    s++;
				element[i]=(char *)malloc(9*sizeof(char));
				strcpy(element[i],"<etoile>");
				i++;
				remplir_elements_ast(e->fg);
				i++;
				element[i]=(char *)malloc(10*sizeof(char));
				strcpy(element[i],"</etoile>");
				break;

		case CONCAT:	s++;
				element[i]=(char *)malloc(9*sizeof(char));
				strcpy(element[i],"<concat>");
				i++;
				remplir_elements_ast(e->fg);
				i++;
				remplir_elements_ast(e->fd);
				i++;
				element[i]=(char *)malloc(10*sizeof(char));
				strcpy(element[i],"</concat>");
				break;	
	
		case UNION:	s++;
				element[i]=(char *)malloc(8*sizeof(char));
				strcpy(element[i],"<union>");
				i++;
				remplir_elements_ast(e->fg);
				i++;
				remplir_elements_ast(e->fd);
				i++;
				element[i]=(char *)malloc(9*sizeof(char));
				strcpy(element[i],"</union>");
				break;	 
}
}
}

void imprimer_ast(ast *e){
   FILE *fc;
   int j;
   char car;
   fc=fopen("ast.xml","w+");
   fprintf(fc,"<ast>\n");
   remplir_elements_ast(e);
   for(j=0;j<=i;j++)
	fprintf(fc,"%s\n",element[j]);
   fprintf(fc,"</ast>\n");
   fclose(fc);
   i=0;
}

int contient_epsilon(ast *e){
     switch(e->type){
	case EPSILON:
	case ETOILE: 	return 1;
	case A:
	case B:
	case VIDE: 	return 0;
	case UNION: 	return contient_epsilon(e->fg) || contient_epsilon(e->fd);
	case CONCAT: 	return contient_epsilon(e->fg) && contient_epsilon(e->fd);
}
}

ast* residuel(char s,ast* e){
   switch(e->type){
	case EPSILON:
	case VIDE: 	return creer_vide();
			
	case A:		if(s=='a')
			  return creer_epsilon();
			else  	
			  return creer_vide();
			
	case B:		if(s=='b')
		          return creer_epsilon();
			else  	
			  return creer_vide();
			
	case CONCAT:	if(contient_epsilon(e->fg))
			  return creer_union(creer_concat(residuel(s,e->fg),e->fd),residuel(s,e->fd));
			else
 			  return creer_concat(residuel(s,e->fg),e->fd);
			
	case UNION:	return creer_union(residuel(s,e->fg),residuel(s,e->fd));
			
	case ETOILE:	return creer_concat(residuel(s,e->fg),e);
			
}
}

int appartient(char *w,ast *e){
   ast* t=(ast *)malloc(sizeof(ast));
   t=copier_ast(e);
   int k,j;
   switch(t->type){

	case EPSILON :	if(strcmp(w,"1")==0) return 1;
			else return 0;
			
	case VIDE:	return 0;
			
	case A:		if(strcmp(w,"a")==0) return 1;
			else return 0;
			
	case B: 	if(strcmp(w,"b")==0) return 1;
			else return 0;
			
	case ETOILE:	  if(strcmp(w,"1")==0)
			   return 1;
			  else{
			  for(k=0;k<strlen(w);k++){
			  t=residuel(w[k],t);
			  if(est_vide(t)) return 0;
			  }
		        if(contient_epsilon(t))
			  return 1;
		        else
			  return 0;}
	case CONCAT: 
	case UNION:     if(strcmp(w,"1") == 0)
			   j=0;
			else
			   j= strlen(w);
			for(k=0;k<j;k++){
			   t=residuel(w[k],t);
			   if(est_vide(t))
			   return 0;
			}
			if(contient_epsilon(t))
			   return 1;
			else
			   return 0;
}
}

int est_vide(ast* e){
   switch(e->type){
	case VIDE: 	return 1;
	case A:		
	case B:		
	case EPSILON:	
	case ETOILE:    return 0;
	case UNION:	return est_vide(e->fg) && est_vide(e->fd);
	case CONCAT:	return est_vide(e->fg) || est_vide(e->fd);	
}
}

int est_epsilon(ast* e){
  switch(e->type){
	case A:
	case B:
	case VIDE:	return 0;
	case EPSILON:	return 1;
	case ETOILE:	if(est_epsilon(e->fg)|| est_vide(e->fg))
			   return 1;
			else 
			   return 0;
	case UNION:	if(est_vide(e->fg)){
			  if(est_epsilon(e->fd))
				return 1;
			}
			else if(est_vide(e->fd)){
			  if(est_epsilon(e->fg))
				return 1;
			}
			else
			  return 0;
	case CONCAT:	return est_epsilon(e->fg) && est_epsilon(e->fd);
	
}
}

int est_symbole(ast* e){
  switch(e->type){
	case VIDE:
	case EPSILON:	
	case ETOILE:	return 0;
	case A:
	case B: 	return 1;
	case UNION:	if(est_symbole(e->fg)){
			   if(est_vide(e->fd))
				return 1;
			   else
				return 0;	
			}
			else if(est_symbole(e->fd)){
			   if(est_vide(e->fg))
				return 1;
			   else
				return 0;	
			}
			else
			   return 0;
	case CONCAT:	if(est_symbole(e->fg)){
			   if(est_epsilon(e->fd))
				return 1;
			   else
				return 0;	
			}
			else if(est_symbole(e->fd)){
			   if(est_epsilon(e->fg))
				return 1;
			   else
				return 0;	
			}
			else
			   return 0;
}	
}

int est_single(ast* e){
   switch(e->type){
	case A:
	case B:
	case EPSILON: 	return 1;
	case VIDE: 	return 0;
	case ETOILE:	if(est_vide(e->fg) || est_epsilon(e->fg))
			   return 1;
			else
			   return 0;
	case UNION:	if(est_single(e->fg)){
			   if(est_vide(e->fd))
				return 1;
			   else
				return 0;	
			}
			else if(est_single(e->fd)){
			   if(est_vide(e->fg))
				return 1;
			   else
				return 0;	
			}
			else
			   return 0;
	case CONCAT:	return est_single(e->fd) && est_single(e->fg); 
}
}

int est_fini(ast *e){
   switch(e->type){
	case VIDE:
	case A:
	case B:
	case EPSILON: 	return 1;
	case ETOILE:	if(est_vide(e->fg) || est_epsilon(e->fg))
			   return 1;
			else
			   return 0;
	case UNION:	return est_fini(e->fg) && est_fini(e->fd);
	case CONCAT:	return est_fini(e->fg) && est_fini(e->fd);
}
}

int est_infini(ast* e){
   switch(e->type){
	case A:
	case B:
	case VIDE:
	case EPSILON:	return 0;
	case ETOILE:	if(est_vide(e->fg) || est_epsilon(e->fg))
			   return 0;
			else
			   return 1;
	case UNION:	return est_infini(e->fg) || est_infini(e->fd);
	case CONCAT:	return est_infini(e->fg) || est_infini(e->fd);
	
}
}

int size(ast* e){
  remplir_elements_ast(e);
  return s;
}
