#include <stdio.h>
#include <stdlib.h>

/* Sucre syntaxique */
#define AND &&
#define OR ||
#define ISNOT !=
#define NOT !
#define then

typedef enum { FALSE, TRUE} bool;

int cpt_malloc_Liste=0;
int cpt_malloc_LListe=0;
/* Listes chaînées de base. */
typedef struct Bloc
{
    int nombre;
    struct Bloc *suivant;
} Bloc;

typedef Bloc *Liste ;


Liste ajoute(int x, Liste l)
{
    cpt_malloc_Liste++;
    Liste tmp = (Liste) malloc(sizeof(Bloc)) ;
    tmp->nombre = x ;
    tmp->suivant = l ;
    return tmp ;
}

void empile(int x, Liste* L){
    *L=ajoute(x, *L);
}
bool estVide(Liste l)
{
    return l == NULL ;
}

Liste suite(Liste l)
{
    return l->suivant ;
}

int premier(Liste l)
{
    return l->nombre ; 
}

void affiche_iter(Liste l)
{
    Liste L2 = l;
    while(!estVide(L2))
    {
        printf("%d ", premier(L2));
        L2 = suite(L2);
    }
    printf("\n");
}




/*************************************************/
/*                                               */
/*         Permutations                          */
/*                                               */
/*************************************************/


typedef struct BlocListe
{
    Liste liste;
    struct BlocListe *suivant;
} BlocListe;

typedef BlocListe *LListe ;



LListe lajoute(Liste x, LListe LL) {
    cpt_malloc_LListe++;
    LListe tmp = (LListe) malloc(sizeof(BlocListe)) ;
    tmp->liste = x ;
    tmp->suivant = LL ;
    return tmp;
}


LListe lconcat(LListe LL1, LListe LL2) {
    if(LL1 == NULL) { return LL2; }
    return lajoute(LL1->liste, lconcat(LL1->suivant, LL2));
}

LListe lconcat_opti(LListe *ll1,LListe* ll2){
    if(ll2!=NULL){
        if(!(*ll1)){
            return *ll2;}
        else{
        LListe *R=ll1;
        while((*R)!=NULL){
            R=&(**R).suivant;
        }
        (*R)=*ll2;
        }
    }
    return *ll1;
}


LListe AETTL(int x, LListe L) {
    if(L == NULL) {return NULL; }
    return lajoute(ajoute(x, L->liste), AETTL(x, L->suivant));
}

LListe AETTL_opti(int x, LListe L){
    LListe R=L;
    while(R!=NULL){
        empile(x,&(*R).liste);
        R=(*R).suivant;
    }
    return L;
}

LListe ATP(int x, Liste L) {
    if(L == NULL) { 
        return lajoute(ajoute(x, NULL), NULL);
    }
    return lajoute(ajoute(x, L), AETTL_opti(L->nombre, ATP(x, L->suivant)));
    //return lajoute(ajoute(x, L), AETTL(L->nombre, ATP(x, L->suivant)));
}

LListe ATLTP(int x, LListe LL) {
    if(LL == NULL) { return NULL;  } 
    //enleve encore un malloc
    else if (LL->liste==NULL){
        empile(x,&LL->liste);
        LListe Q= ATLTP(x,LL->suivant);
        LL->suivant=NULL;
        return lconcat_opti(&LL,&Q);
    }
    else{
    LListe atp=ATP(x, LL->liste);
    LListe atptl=ATLTP(x, LL->suivant);
    return lconcat_opti(&atp, &atptl);
    //return lconcat(ATP(x, LL->liste), ATLTP(x, LL->suivant));
    }
}


LListe Permutations(int n) {
    if(n == 0) { return lajoute(NULL, NULL); }
    return ATLTP(n, Permutations(n-1));
}


void afficheLL(LListe Q){
    while(Q != NULL) {
        affiche_iter(Q->liste);
        Q = Q->suivant;
    }
}

/*********************************************/
/*                                           */
/*             PERMU COMPRESSED              */
/*                                           */
/*********************************************/


Liste concat_opti(Liste* l1,Liste* l2){
    if((*l2)!=NULL){
        if(!(*l1)){
            return *l2;}
        else{
        Liste* R=l1;
        while((*R)!=NULL){
            R=&(**R).suivant;
        }
        *R=*l2;
        }
    }
    return *l1;
}

Liste concat(Liste L1, Liste L2){
    if(L1 == NULL) { return L2; }
    return ajoute(L1->nombre, concat(L1->suivant, L2));
}

int longueur_rec (Liste l)
{
    if (l == NULL)
         return 0 ;
    else return (1 + longueur_rec(l->suivant)) ;
}

Liste concat_b(Liste L1,Liste L2,int n){
    if (n==0){
        return concat_opti(&L1,&L2);
    }
    else{
        return ajoute(L1->nombre,concat_b(L1->suivant,L2,n-1));
    }
}
Liste conca_final(Liste L1, Liste L2, int n){
    int i =longueur_rec(L1);
    if (i<n){
        return concat(L1,L2);
    }
    else{
        return concat_b(L1,L2,i-n);
    }
}

/*********************************************/
/*                                           */
/*      compression de que de LListes        */
/*                                           */
/*********************************************/

LListe ATP_compressor_only_LListe(int x,LListe L){
    Liste* Q=&L->liste;
    Liste R=NULL;
    Liste* W=&R;
    Liste nouv=NULL;
    empile(x,Q);
    Q=&(*Q)->suivant;
    while(*Q!=NULL){
        empile((*Q)->nombre,&(*W));
        Q=&(*Q)->suivant;
        W=&(*W)->suivant;
        nouv=concat(R,ajoute(x,*Q));
        L=lajoute(nouv,L);
    }
    return L;
}

LListe ATLTP_compressor_only_LListes(int x, LListe LL) {
    if (LL == NULL) {
        return NULL;
    }

    LListe firstATPResult = ATP_compressor_only_LListe(x, LL);

    LListe restATPResults = ATLTP_compressor_only_LListes(x, LL->suivant);
    LL->suivant = restATPResults;

    return firstATPResult;
}

LListe permutations_compressor_only_LListes(int n){
    if (n==0){
        return lajoute(NULL,NULL);
    }
    else{
        return ATLTP_compressor_only_LListes(n,permutations_compressor_only_LListes(n-1));
    }
}

/*********************************************/
/*                                           */
/*      compression de Listes et LListes     */
/*                                           */
/*********************************************/

LListe ATP_compressor(int x,LListe L, int i){
    Liste* Q=&L->liste;
    Liste R=NULL;
    //Liste* W=&R;
    Liste nouv=NULL;
    empile(x,Q);
    Q=&(*Q)->suivant;
    while(*Q!=NULL){
        empile((*Q)->nombre,&R);
        Q=&(*Q)->suivant;
        //W=&(*W)->suivant;
        nouv=conca_final(R,ajoute(x,(*Q)), i);
        L=lajoute(nouv,L);
    }
    return L;
}

LListe ATLTP_compressor_bis(int x, LListe LL, int i) {
    if (LL == NULL) {
        return NULL;
    }
    LListe firstATPResult = ATP_compressor(x, LL,i);

    LListe restATPResults = ATLTP_compressor_bis(x, LL->suivant,i);
    LL->suivant = restATPResults;

    return firstATPResult;
}

LListe ATLTP_comp_final(int x, LListe LL){
    int i=longueur_rec(LL->liste);
    return ATLTP_compressor_bis(x,LL,i);
}

//ON SE BASE SUR LE FAIT QUE TOUTES LES SOUS LISTES SONT DE MEME TAILLE
// ce qui est logique parce qu'elle sont des permutations de n elements.

LListe permutations_compressor(int n){
    if (n==0){
        return lajoute(NULL,NULL);
    }
    else{
        return ATLTP_comp_final(n,permutations_compressor(n-1));
    }
}

int main() {
    afficheLL(Permutations(4));
    printf("malloc Listes: %d \n malloc LListes:%d \n", cpt_malloc_Liste, cpt_malloc_LListe);
    affichePermutations(6);
    return 0;
}