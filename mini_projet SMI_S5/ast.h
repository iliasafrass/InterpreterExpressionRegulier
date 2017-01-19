
typedef struct ast{
enum{CONCAT,ETOILE,UNION,A,B,EPSILON,VIDE}type;
struct ast *fg;
struct ast *fd;
}ast;



ast* creer_a();
ast* creer_b();
ast* creer_epsilon();
ast* creer_vide();
ast* creer_union(ast* ,ast* );
ast* creer_concat(ast* ,ast* );
ast* creer_etoile(ast* );
ast* copier_ast(ast* );
void liberer_ast(ast* );
void remplir_elements_ast(ast* );
void imprimer_ast(ast* );
int contient_epsilon(ast* );
ast* residuel(char,ast* );
int appartient(char* ,ast *);
int est_vide(ast* );
int est_epsilon(ast* );
int est_symbole(ast* );
int est_single(ast* );
int est_fini(ast* );
int est_infini(ast* );
int size(ast* );