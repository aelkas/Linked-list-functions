#include <stdio.h>
#include <stdlib.h>

#include <time.h> // Utilisé pour mésurer des temps d'execution.

/*************************************************/
/*                                               */
/*                sucre syntaxique               */
/*                                               */
/*************************************************/

#define AND &&
#define OR ||
#define ISNOT !=
#define NOT !
#define then

typedef enum { FALSE, TRUE} bool;

/*************************************************/
/*                                               */
/*          definition type liste                */
/*                                               */
/*************************************************/

typedef struct Bloc
{
    int nombre;
    struct Bloc *suivant;
} Bloc;

typedef Bloc *Liste ;

/*************************************************/
/*                                               */
/*                predeclarations                */
/*                                               */
/*************************************************/

/* initialise une Liste � vide */
void initVide(Liste *L);

/* renvoie 1 si la Liste en parametre est vide, 0 sinon */
bool estVide(Liste l);

/* renvoie le premier element de la Liste en parametre */
int premier(Liste l);

/* renvoie une nouvelle Liste correspondant a celle en parametre, avec l'element x ajoute en haut de la pile */
Liste ajoute(int x, Liste l);

/* modifie la Liste en parametre: x est ajoute comme premier element */
void empile(int x, Liste* L);

/* renvoie une nouvelle Liste correspondant a celle en parametre sans son premier element */
Liste suite(Liste l);

/* modifie la Liste en parametre: le premier element est retire */
void depile(Liste* l);

/* affichage simple en recursif et en iteratif */
void affiche_rec(Liste l);
void affiche_iter(Liste l);

/* longueur en recursif et en iteratif */
int longueur_rec (Liste l);
int longueur_iter (Liste l);

/*  Elimination du dernier element en recursif et en iteratif  */
/*  VD est la sousprocedure utilitaire de la version recursive */
void VD (Liste *L);
void VireDernier_rec (Liste *L);
void VireDernier_iter (Liste *L);


/*************************************************/
/*                                               */
/*                briques de base                */
/*                                               */
/*************************************************/


void initVide( Liste *L)
{
    *L = NULL ;
}

bool estVide(Liste l)
{
    return l == NULL ;
}

int premier(Liste l)
{
    return l->nombre ; 
}

Liste ajoute(int x, Liste l)
{
    Liste tmp = (Liste) malloc(sizeof(Bloc)) ;
    tmp->nombre = x ;
    tmp->suivant = l ;
    return tmp ;
}

void empile(int x, Liste *L)
{
      *L = ajoute(x,*L) ; 
}

Liste suite(Liste l)
{
    return l->suivant ;
}


void depile(Liste *L)
{
    Liste tmp = *L ;
    *L = suite(*L) ;
    free(tmp) ;
}

/*************************************************/
/*                                               */
/*     Affiche, avec les briques de base         */
/*                                               */
/*************************************************/

void affiche_rec(Liste l)
{
    if(estVide(l))
        printf("\n");
    else
    {
        printf("%d ", premier(l));
        affiche_rec(suite(l));
    }
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
/*     Longueur, sans les briques de base        */
/*                                               */
/*************************************************/

int longueur_rec (Liste l)
{
    if (l == NULL)
         return 0 ;
    else return (1 + longueur_rec(l->suivant)) ;
}


int longueur_iter (Liste l)
{
    Liste P = l;
    int cpt = 0 ;
    while (P ISNOT NULL)
    {   P = P->suivant ;
        cpt++ ;
    }
    return cpt ;
}

/*************************************************/
/*                                               */
/*       VireDernier,                            */
/*               sans les briques de base,       */
/*               ni le "->"                      */
/*                                               */
/*************************************************/

void VD (Liste *L)
          // *L non NULL ie liste non vide
{
     if ( ((**L).suivant) == NULL )
            depile(L) ;   // moralement : depile(& (*L)) ;
     else VD (& ( (**L).suivant )) ;
}

void VireDernier_rec (Liste *L)
{
     if ( (*L) ISNOT NULL )
          VD(L);        // moralement : VD(& (*L)) ;
}

void VireDernier_iter (Liste *L)
{
    if ( (*L) ISNOT NULL)
    {
        while ( ((**L).suivant) ISNOT NULL )
                 L = & ( (**L).suivant ) ;
        free(*L) ;
        *L = NULL ;
     }
}


/*************************************************/
/*                                               */
/*       Libere la memoire                       */
/*                                               */
/*************************************************/

void VideListe(Liste *L)
{
    if(NOT(estVide(*L)))
    {
        depile(L);
        VideListe(L);
    }
      
}





/*************************************************/
/*************************************************/
/*                                               */
/*           Projet                              */
/*                                               */
/*************************************************/
/*************************************************/





/*************************************************/
/*                                               */
/*   Infrastructure de tests                     */
/*                                               */
/*************************************************/


/* 
Pour tester notre code on a crée une infrastructure minimale de tests.
Cela nous permet de garder de l'information utile, comme le temps d'execution,
et de lancer des tests au choix. 
*/

typedef bool (*test)();

typedef struct block_test {
    test tst;
    int code; // 0 pour échéc, 1 pour réussite, 2 pour skip.
    const char *nom;
    double temps_execution;
    struct block_test *suivant;

} block_test;

typedef block_test *suite_tests;


void empile_test(suite_tests *Suite, test tst, const char* nom) {
    suite_tests tmp = (suite_tests)malloc(sizeof(block_test));
    tmp -> tst = tst;
    tmp -> nom = nom;
    tmp -> code = 0;
    tmp -> suivant = *Suite;
    tmp -> temps_execution = 0;
    *Suite = tmp;
}


void lance_tests(suite_tests suite) {
    suite_tests P = suite;

    printf("\033[95m\033[1mRESULTATS TESTS\033[00m\n"); // On augmente la lisbilité des résultats avec des couleurs ANSI.
    printf("-----------------------------------------\n");

    while(P != NULL) {
        printf("\033[95m\033[96m%s\033[00m => ", P->nom);
        if(P -> code == 2) {
            printf("\033[95m\033[93mSkip!\033[00m\n"); 
        }
        else {
            clock_t t_debut, t_fin;

            t_debut = clock();
            bool resultat = (P -> tst)();
            t_fin = clock();
            P -> temps_execution = ((double) (t_fin - t_debut)) / CLOCKS_PER_SEC;
            

            if (resultat) {
                P -> code = 1;
                printf("\033[95m\033[92mReussite!\033[00m\n"); // Certains terminaux n'affichent pas les accents correctement.
            }
            else {
                P -> code = 0;
                printf("\033[95m\033[91mEchec!\033[00m\n"); // On auraît bien aimé mettre les accents.
            }

            printf("Execution en %lf s\n", P -> temps_execution);
        }
        P = P -> suivant;
    }

    printf("-----------------------------------------\n");
}

void saute_dernier(suite_tests suite) {
    suite -> code = 2;
}


void libere_tests(suite_tests suite) {
    suite_tests P = suite;
    while(P != NULL) {
        suite_tests tmp = P;
        P = P -> suivant;
        free(tmp);
    }
}


/* Cette fonction est utile pour les tests avec des Listes-Piles. */
bool egale(Liste L1, Liste L2) {
    Liste P1 = L1;
    Liste P2 = L2;
    while (P1 != NULL && P2 != NULL)
    {
        if (premier(P1) != premier(P2)){
            return FALSE;
        }
 
        P1 = suite(P1);
        P2 = suite(P2);
    }
 
    return (P1 == NULL && P2 == NULL);
}





/*************************************************/
/*                                               */
/*         Listes-Piles                          */
/*                                               */
/*************************************************/



/***********************/
/* UnPlusDeuxEgalTrois */
/***********************/


bool UnPlusDeuxEgalTrois(Liste L) {
    Liste P = L;
    int compte = 0;
    int acc = 0;
    while ( !(estVide(P)) && compte < 2 ) {
        acc += premier(P);
        compte ++;
        P = suite(P);
    }
    if ( estVide(P) ) {
        return acc == 0;
    }
    return acc == premier(P);
}

bool test_UnPlusDeuxEgalTrois() {
    /*[2, 3, 5, 4, 2] -> TRUE*/
    Liste L_in = NULL;
    empile(2, &L_in); empile(4, &L_in); empile(5, &L_in); empile(3, &L_in); empile(2, &L_in);
    bool resultat = UnPlusDeuxEgalTrois(L_in);
    
    VideListe(&L_in);
    return resultat;
}

bool test_UnPlusDeuxEgalTrois_contreexemple() {
    /*[2, 3, 3, 4, 2] -> TRUE*/
    Liste L_in = NULL;
    empile(2, &L_in); empile(4, &L_in); empile(3, &L_in); empile(3, &L_in); empile(2, &L_in);
    bool resultat = UnPlusDeuxEgalTrois(L_in);
    
    VideListe(&L_in);
    return !resultat;
}

bool test_UnPlusDeuxEgalTrois_vide() {
    /*[] -> TRUE*/
    Liste L_in = NULL;
    bool resultat = UnPlusDeuxEgalTrois(L_in);
    
    VideListe(&L_in);
    return resultat;
}

bool test_UnPlusDeuxEgalTrois_couple() {
    /*[2, 2] -> FALSE*/
    Liste L_in = NULL;
    empile(2, &L_in); empile(2, &L_in);
    bool resultat = UnPlusDeuxEgalTrois(L_in);
    
    VideListe(&L_in);
    return !resultat;
}



/**************/
/* Croissante */
/**************/


bool Croissante(Liste L) {
    Liste P = L;
    while ( !(estVide(P)) && !(estVide(suite(P))) ) {
        if ( premier(P) >= premier(suite(P)) ) { return FALSE; }
        P = suite(P);
    }
    return TRUE;
}

bool test_Croissante_vide() {
    Liste L_in = NULL;
    return Croissante(L_in);
}

bool test_Croissante_non_stricte() {
    /*[1, 2, 2, 3] -> FALSE*/
    Liste L_in = NULL;
    empile(3, &L_in); empile(2, &L_in); empile(2, &L_in); empile(1, &L_in);
    bool  resultat = !Croissante(L_in);

    VideListe(&L_in);
    free(L_in);
    return resultat;
}

bool test_Croissante_singleton() {
    /*[3] -> TRUE*/
    Liste L_in = NULL;
    empile(3, &L_in);
    bool  resultat = Croissante(L_in);

    VideListe(&L_in);
    free(L_in);
    return resultat;
}

bool test_Croissante() {
    /*[1, 3, 4, 5] -> TRUE*/
    Liste L_in = NULL;
    empile(5, &L_in); empile(4, &L_in); empile(3, &L_in); empile(1, &L_in);
    bool resultat = Croissante(L_in);

    VideListe(&L_in);
    free(L_in);
    return resultat;
}



/**********************/
/* NombreMemePosition */
/**********************/


/* Pour ne pas réecrire les tests pour chaque version de la fonction on crée une version abstraite.*/
bool template_test_NombreMemePosition(int (*fonction)(Liste, Liste)) {
    /* [1, 0, 2, 0], [1, 3, 2, 0] -> 3 */
    Liste L1 = NULL;
    Liste L2 = NULL;

    empile(0, &L1); empile(2, &L1); empile(0, &L1); empile(1, &L1);
    empile(0, &L2); empile(2, &L2); empile(3, &L2); empile(1, &L2);

    bool resultat = ((*fonction)(L1, L2) == 3);
    VideListe(&L1);
    VideListe(&L2);

    return resultat;
}

bool template_test_NombreMemePosition_differentes_longueurs(int (*fonction)(Liste, Liste)) {
    /* [1, 0, 2, 0], [1, 3] -> 1 */
    Liste L1 = NULL;
    Liste L2 = NULL;

    empile(0, &L1); empile(2, &L1); empile(0, &L1); empile(1, &L1);
    empile(3, &L2); empile(1, &L2);

    bool resultat = ((*fonction)(L1, L2) == 1);
    VideListe(&L1);
    VideListe(&L2);

    return resultat;
}

bool template_test_NombreMemePosition_un_vide(int (*fonction)(Liste, Liste)) {
    /* [1, 0, 2, 0], [] -> 0 */
    Liste L1 = NULL;
    Liste L2 = NULL;

    empile(0, &L1); empile(2, &L1); empile(0, &L1); empile(1, &L1);

    bool resultat = ((*fonction)(L1, L2) == 0);
    VideListe(&L1);
    VideListe(&L2);

    return resultat;
}

bool template_test_NombreMemePosition_deux_vides(int (*fonction)(Liste, Liste)) {
    /* [], [] -> 0 */
    Liste L1 = NULL;
    Liste L2 = NULL;

    bool resultat = ((*fonction)(L1, L2) == 0);
    VideListe(&L1);
    VideListe(&L2);

    return resultat;
}

bool template_test_NombreMemePosition_egales(int (*fonction)(Liste, Liste)) {
    /* [1, 0, 2, 0], [1, 0, 2, 0] -> 0 */
    Liste L1 = NULL;
    Liste L2 = NULL;

    empile(0, &L1); empile(2, &L1); empile(0, &L1); empile(1, &L1);
    empile(0, &L2); empile(2, &L2); empile(0, &L2); empile(1, &L2);

    bool resultat = ((*fonction)(L1, L2) == 4);
    VideListe(&L1);
    VideListe(&L2);

    return resultat;
}


/* Version récursive */
int NombreMemePositionRec(Liste L1, Liste L2) {
    if(estVide(L1) || estVide(L2)) { return 0; }
    int resultat_suite = NombreMemePositionRec(suite(L1), suite(L2));
    if (premier(L1) == premier(L2))
        return 1 + resultat_suite;
    return resultat_suite;
}

bool test_NombreMemePositionRec() {
    return template_test_NombreMemePosition(&NombreMemePositionRec);
}

bool test_NombreMemePositionRec_differentes_longueurs() {
    return template_test_NombreMemePosition_differentes_longueurs(&NombreMemePositionRec);
}

bool test_NombreMemePositionRec_un_vide() {
    return template_test_NombreMemePosition_un_vide(&NombreMemePositionRec);
}

bool test_NombreMemePositionRec_egales() {
    return template_test_NombreMemePosition_egales(&NombreMemePositionRec);
}

bool test_NombreMemePositionRec_deux_vides() {
    return template_test_NombreMemePosition_deux_vides(&NombreMemePositionRec);
}


/* Version itérative */
int NombreMemePositionIter(Liste L1, Liste L2) {
    int acc = 0;
    Liste P1 = L1;
    Liste P2 = L2;
    while(!estVide(P1) && !estVide(P2)) {
        if(premier(P1) == premier(P2)) { acc++; }
        P1 = suite(P1);
        P2 = suite(P2);
    }
    return acc;
}

bool test_NombreMemePositionIter() {
    return template_test_NombreMemePosition(&NombreMemePositionIter);
}

bool test_NombreMemePositionIter_differentes_longueurs() {
    return template_test_NombreMemePosition_differentes_longueurs(&NombreMemePositionIter);
}

bool test_NombreMemePositionIter_un_vide() {
    return template_test_NombreMemePosition_un_vide(&NombreMemePositionIter);
}

bool test_NombreMemePositionIter_egales() {
    return template_test_NombreMemePosition_egales(&NombreMemePositionIter);
}

bool test_NombreMemePositionIter_deux_vides() {
    return template_test_NombreMemePosition_deux_vides(&NombreMemePositionIter);
}


/* Version récursive terminale avec sous-fonction */
int AuxNombreMemePositionFonc(Liste L1, Liste L2, int acc) {
    if(estVide(L1) || estVide(L2)) { return acc; }
    if (premier(L1) == premier(L2)) { 
        acc++; 
    }
    return AuxNombreMemePositionFonc(suite(L1), suite(L2), acc);
}

int NombreMemePositionRecTerFonc(Liste L1, Liste L2) {
    return AuxNombreMemePositionFonc(L1, L2, 0);
}

bool test_NombreMemePositionRecTerFonc() {
    return template_test_NombreMemePosition(&NombreMemePositionRecTerFonc);
}

bool test_NombreMemePositionRecTerFonc_differentes_longueurs() {
    return template_test_NombreMemePosition_differentes_longueurs(&NombreMemePositionRecTerFonc);
}

bool test_NombreMemePositionRecTerFonc_un_vide() {
    return template_test_NombreMemePosition_un_vide(&NombreMemePositionRecTerFonc);
}

bool test_NombreMemePositionRecTerFonc_egales() {
    return template_test_NombreMemePosition_egales(&NombreMemePositionRecTerFonc);
}

bool test_NombreMemePositionRecTerFonc_deux_vides() {
    return template_test_NombreMemePosition_deux_vides(&NombreMemePositionRecTerFonc);
}


/* Version récursive terminale avec sous-procédure*/
void AuxNombreMemePositionProc(Liste L1, Liste L2, int *acc) {
    if(estVide(L1) || estVide(L2)) { return; } // Cas de base
    if(premier(L1) == premier(L2)) { (*acc)++; }
    AuxNombreMemePositionProc(suite(L1), suite(L2), acc);
}

int NombreMemePositionRecTerProc(Liste L1, Liste L2) {
    int acc = 0;
    AuxNombreMemePositionProc(L1, L2, &acc);
    return acc;
}

bool test_NombreMemePositionRecTerProc() {
    return template_test_NombreMemePosition(&NombreMemePositionRecTerProc);
}

bool test_NombreMemePositionRecTerProc_differentes_longueurs() {
    return template_test_NombreMemePosition_differentes_longueurs(&NombreMemePositionRecTerProc);
}

bool test_NombreMemePositionRecTerProc_un_vide() {
    return template_test_NombreMemePosition_un_vide(&NombreMemePositionRecTerProc);
}

bool test_NombreMemePositionRecTerProc_egales() {
    return template_test_NombreMemePosition_egales(&NombreMemePositionRecTerProc);
}

bool test_NombreMemePositionRecTerProc_deux_vides() {
    return template_test_NombreMemePosition_deux_vides(&NombreMemePositionRecTerProc);
}



/***************/
/* VireDernier */
/***************/


bool template_test_FonctVireDernier(Liste (*fonction)(Liste)) {
    /*[1, 2, 3, 4] -> [1, 2, 3]*/
    Liste L_in = NULL;
    Liste L_resultat = NULL;

    empile(4, &L_in); empile(3, &L_in); empile(2, &L_in); empile(1, &L_in);
    empile(3, &L_resultat); empile(2, &L_resultat); empile(1, &L_resultat);

    return egale((*fonction)(L_in), L_resultat);
}

bool template_test_FonctVireDernier_vide(Liste (*fonction)(Liste)) {
    /*[] -> []*/
    Liste L_in = NULL;
    Liste L_resultat = NULL;

    return egale((*fonction)(L_in), L_resultat);
}

bool template_test_FonctVireDernier_singleton(Liste (*fonction)(Liste)) {
    /*[1] -> []*/
    Liste L_in = NULL;
    Liste L_resultat = NULL;

    empile(1, &L_in);

    return egale((*fonction)(L_in), L_resultat);
}


/* Version récursive */
Liste FonctVireDernierRec(Liste L) {
    if(estVide(L)) return NULL; // Cette ligne ne s'execute que si L était originalement vide.
    if(estVide(suite(L))) {
        free(L);
        return NULL;
    }
    return ajoute(premier(L), FonctVireDernierRec(suite(L)));
}

bool test_FonctVireDernierRec() {
    return template_test_FonctVireDernier(&FonctVireDernierRec);
}

bool test_FonctVireDernierRec_vide() {
    return template_test_FonctVireDernier_vide(&FonctVireDernierRec);
}

bool test_FonctVireDernierRec_singleton() {
    return template_test_FonctVireDernier_singleton(&FonctVireDernierRec);
}


/* Version itérative */
Liste FonctVireDernierIter(Liste L) {
    if(estVide(L) || estVide(suite(L))) return NULL;
    
    Liste N = (Liste) malloc(sizeof (Bloc));
    Liste debut = N;
    while( !estVide(suite(L)) )  { // Au début L n'est jamais vide. On a traité ce cas avant.
        N -> nombre = premier(L);
        if( !estVide(suite(suite(L))) ) {
            N -> suivant = (Liste) malloc(sizeof (Bloc));
        }
        else {
            N -> suivant = NULL;
        }
        L = suite(L);
        N = suite(N);
    }
    return debut;
}

bool test_FonctVireDernierIter() {
    return template_test_FonctVireDernier(&FonctVireDernierIter);
}

bool test_FonctVireDernierIter_vide() {
    return template_test_FonctVireDernier_vide(&FonctVireDernierIter);
}

bool test_FonctVireDernierIter_singleton() {
    return template_test_FonctVireDernier_singleton(&FonctVireDernierIter);
}



/***************************/
/* AjouteDevantPremierZero */
/***************************/


void AjouteDevantPremierZero(Liste *L, int x) {
    if(estVide(*L) || premier(*L) == 0) {
        empile(x, L);
        return;
    }

    Liste P = *L;
    Liste tmp = (Liste) malloc(sizeof (Bloc));
    tmp -> nombre = x;
    while (!estVide(suite(P))) { // On a vérifié que P n'est pas vide au début avant
        if(premier(suite(P)) == 0) {
            tmp -> suivant = suite(P);
            P -> suivant = tmp;
            return;
        }
        P = suite(P);
    }

    P -> suivant = tmp;
    tmp -> suivant = NULL;
}

bool test_AjouteDevantPremierZero() {
    /* [1, 0, 2, 0] -> [1, 42, 0, 2, 0] */
    Liste L_in = NULL;
    Liste L_resultat = NULL;

    empile(0, &L_in); empile(2, &L_in); empile(0, &L_in); empile(1, &L_in);
    empile(0, &L_resultat); empile(2, &L_resultat); empile(0, &L_resultat); empile(42, &L_resultat); empile(1, &L_resultat);

    AjouteDevantPremierZero(&L_in, 42);
    bool resultat = egale(L_in, L_resultat);
    VideListe(&L_in);
    VideListe(&L_resultat);

    return resultat;
}

bool test_AjouteDevantPremierZero_sansZero() {
    /* [1, 2, 3] -> [1, 2, 3, 4] */
    Liste L_in = NULL;
    Liste L_resultat = NULL;

    empile(3, &L_in); empile(2, &L_in); empile(1, &L_in);
    empile(4, &L_resultat); empile(3, &L_resultat); empile(2, &L_resultat); empile(1, &L_resultat);

    AjouteDevantPremierZero(&L_in, 4);
    bool resultat = egale(L_in, L_resultat);
    VideListe(&L_in);
    VideListe(&L_resultat);

    return resultat;
}

bool test_AjouteDevantPremierZero_vide() {
    /* [] -> [0] */
    Liste L_in = NULL;
    Liste L_resultat = NULL;

    empile(0, &L_resultat);

    AjouteDevantPremierZero(&L_in, 0);
    bool resultat = egale(L_in, L_resultat);
    VideListe(&L_in);
    VideListe(&L_resultat);

    return resultat;
}

bool test_AjouteDevantPremierZero_premierZero() {
    /* [0, 1] -> [3, 0, 1] */
    Liste L_in = NULL;
    Liste L_resultat = NULL;

    empile(1, &L_in); empile(0, &L_in);
    empile(1, &L_resultat); empile(0, &L_resultat); empile(3, &L_resultat);

    AjouteDevantPremierZero(&L_in, 3);
    bool resultat = egale(L_in, L_resultat);
    VideListe(&L_in);
    VideListe(&L_resultat);

    return resultat;
}



/**************************/
/* AjouteDevantDenierZero */
/**************************/


/* Pour ne pas réecrire les tests pour chaque version de la fonction on crée une version abstraite.*/
bool template_test_AjouteDevantDernierZero(void (*fonction)(Liste*, int)) {
    /* [1, 0, 2, 0] -> [1, 0, 2, 42, 0] */
    Liste L_in = NULL;
    Liste L_resultat = NULL;

    empile(0, &L_in); empile(2, &L_in); empile(0, &L_in); empile(1, &L_in);
    empile(0, &L_resultat); empile(42, &L_resultat); empile(2, &L_resultat); empile(0, &L_resultat); empile(1, &L_resultat);

    (*fonction)(&L_in, 42);
    bool resultat = egale(L_in, L_resultat);
    VideListe(&L_in);
    VideListe(&L_resultat);

    return resultat;
}

bool template_test_AjouteDevantDernierZero_sansZero(void (*fonction)(Liste*, int)) {
    /* [1, 2, 3] -> [1, 2, 3, 4] */
    Liste L_in = NULL;
    Liste L_resultat = NULL;

    empile(3, &L_in); empile(2, &L_in); empile(1, &L_in);
    empile(4, &L_resultat); empile(3, &L_resultat); empile(2, &L_resultat); empile(1, &L_resultat);

    (*fonction)(&L_in, 4);
    bool resultat = egale(L_in, L_resultat);
    VideListe(&L_in);
    VideListe(&L_resultat);

    return resultat;
}

bool template_test_AjouteDevantDernierZero_vide(void (*fonction)(Liste*, int)) {
    /* [] -> [0] */
    Liste L_in = NULL;
    Liste L_resultat = NULL;

    empile(0, &L_resultat);

    (*fonction)(&L_in, 0);
    bool resultat = egale(L_in, L_resultat);
    VideListe(&L_in);
    VideListe(&L_resultat);

    return resultat;
}

bool template_test_AjouteDevantDernierZero_premierZero(void (*fonction)(Liste*, int)) {
    /* [0, 1] -> [3, 0, 1] */
    Liste L_in = NULL;
    Liste L_resultat = NULL;

    empile(1, &L_in); empile(0, &L_in);
    empile(1, &L_resultat); empile(0, &L_resultat); empile(3, &L_resultat);

    (*fonction)(&L_in, 3);
    bool resultat = egale(L_in, L_resultat);
    VideListe(&L_in);
    VideListe(&L_resultat);

    return resultat;
}


/* Version récursive non-terminale. */
void AjouteDevantDernierZeroRec_aux(Liste *L, int x, bool *drapeau) {
    // Le drapeau sert une double fonctionalité. Avant arriver au cas de base, il signalise la présence
    // de zéros dans la liste. S'il n'y en a pas, on ajoutera x à la fin de celle-ci (traîtant ainsi le
    // cas de liste vide et celui où il n'y a pas de zéros en même temps). Pendant le dépilement des appels
    // récursifs, le drapeau peut se lire comme << dernier zéro trouvé >>, et donc est actif à partir du
    // premier zéro trouvé dès la fin.

    if(estVide(*L)) {
        if (!(*drapeau)) {
            empile(x, L);
        }
        else {
            *drapeau = FALSE;
        }
    }

    else {
        int valeur_actuelle = premier(*L);

        if(valeur_actuelle == 0) {
            *drapeau = TRUE;
        }

        depile(L);

        AjouteDevantDernierZeroRec_aux(L, x, drapeau); 
        // À partir d'ici lire drapeau comme << Dernier zéro trouvé >>.

        empile(valeur_actuelle, L);

        if(valeur_actuelle == 0 && !(*drapeau)) { // Garde pour n'empiler x que si c'est le dernier zéro.
            empile(x, L);
            *drapeau = TRUE;
        }
    }

}

void AjouteDevantDernierZeroRec(Liste *L, int x) {
    bool drapeau = FALSE; 
    AjouteDevantDernierZeroRec_aux(L, x, &drapeau);
}


bool test_AjouteDevantDernierZeroRec() {
    return template_test_AjouteDevantDernierZero(&AjouteDevantDernierZeroRec);
}

bool test_AjouteDevantDernierZeroRec_premierZero() {
    return template_test_AjouteDevantDernierZero_premierZero(&AjouteDevantDernierZeroRec);
}

bool test_AjouteDevantDernierZeroRec_sansZero() {
    return template_test_AjouteDevantDernierZero_sansZero(&AjouteDevantDernierZeroRec);
}

bool test_AjouteDevantDernierZeroRec_vide() {
    return template_test_AjouteDevantDernierZero_vide(&AjouteDevantDernierZeroRec);
}


/* Version récursive terminale */
void AjouteDevantDernierZeroRecTer_aux(Liste *L, int x, Liste *precedent_dernier_zero) {
    if(estVide(*L)) { // On lance la fonction auxiliere sur une liste non-vide. On ne commence jamais au cas de base.
        Liste tmp = (Liste)malloc(sizeof(Bloc));
        tmp -> nombre = x;
        tmp -> suivant = (*precedent_dernier_zero) -> suivant;
        (*precedent_dernier_zero) -> suivant = tmp;
    }

    else {
        Liste S = suite(*L);
        if(estVide(S)) {
            if(precedent_dernier_zero == NULL) { 
                AjouteDevantDernierZeroRecTer_aux(&S, x, L); // Dans le cas de base precedent_dernier_zero n'est jamais NULL.
            }
            else {
                AjouteDevantDernierZeroRecTer_aux(&S, x, precedent_dernier_zero);
            }
        }
        else {
            if(premier(S) == 0) {
                AjouteDevantDernierZeroRecTer_aux(&S, x, L);
            }
            else {
                AjouteDevantDernierZeroRecTer_aux(&S, x, precedent_dernier_zero);
            }
        }
    }

}

void AjouteDevantDernierZeroRecTer(Liste *L, int x) {
    empile(0, L); // On ajoute temporairement un nouveau élément pour simplifier l'algorithme.
    AjouteDevantDernierZeroRecTer_aux(L, x, NULL);
    depile(L);
}

bool test_AjouteDevantDernierZeroRecTer() {
    return template_test_AjouteDevantDernierZero(&AjouteDevantDernierZeroRecTer);
}

bool test_AjouteDevantDernierZeroRecTer_premierZero() {
    return template_test_AjouteDevantDernierZero_premierZero(&AjouteDevantDernierZeroRecTer);
}

bool test_AjouteDevantDernierZeroRecTer_sansZero() {
    return template_test_AjouteDevantDernierZero_sansZero(&AjouteDevantDernierZeroRecTer);
}

bool test_AjouteDevantDernierZeroRecTer_vide() {
    return template_test_AjouteDevantDernierZero_vide(&AjouteDevantDernierZeroRecTer);
}


/* Version itérative */
void AjouteDevantDernierZeroIter(Liste *L, int x) {
    empile(0, L); // On ajoute un élément temporairement.
    Liste P = *L;
    Liste precedent_dernier_zero = P;
    bool trouvee = FALSE;

    while(!estVide(P)) {
        if(!estVide(suite(P)) && premier(suite(P)) == 0){
            precedent_dernier_zero = P;
            trouvee = TRUE;
        }
        if(estVide(suite(P)) && !trouvee) {
            precedent_dernier_zero = P;
        }

        P = suite(P);
    }

    Liste tmp = (Liste)malloc(sizeof(Bloc));
    tmp -> nombre = x;
    tmp -> suivant = precedent_dernier_zero -> suivant; // precedent_dernier_zero n'est jamais NULL à ce point.
    precedent_dernier_zero -> suivant = tmp;

    depile(L); // On élimine l'élément empilé au début de la fonction.
}

bool test_AjouteDevantDernierZeroIter() {
    return template_test_AjouteDevantDernierZero(&AjouteDevantDernierZeroIter);
}

bool test_AjouteDevantDernierZeroIter_premierZero() {
    return template_test_AjouteDevantDernierZero_premierZero(&AjouteDevantDernierZeroIter);
}

bool test_AjouteDevantDernierZeroIter_sansZero() {
    return template_test_AjouteDevantDernierZero_sansZero(&AjouteDevantDernierZeroIter);
}

bool test_AjouteDevantDernierZeroIter_vide() {
    return template_test_AjouteDevantDernierZero_vide(&AjouteDevantDernierZeroIter);
}



/*******/
/* Tic */
/*******/


void Tic(Liste *L) {
    if(estVide(*L)) return;

    if(premier(*L) == 0) {
        while(!estVide(*L) && premier(*L) == 0) {
            depile(L);
        }
    } 
    else {
        int valeur_actuelle = premier(*L);
        depile(L);
        Tic(L);
        empile(valeur_actuelle, L);
        empile(0, L);
    }
}

bool test_Tic() {
    /*[3, 7, 2, 0, 0, 0, 8, 9, 0, 0, 2, 1] -> [0, 3, 0, 7, 0, 2, 8, 9, 0, 0, 2, 1]*/
    Liste L_in = NULL;
    Liste L_resultat = NULL;

    empile(1, &L_in); empile(2, &L_in); empile(0, &L_in); empile(0, &L_in); empile(9, &L_in);
    empile(8, &L_in); empile(0, &L_in); empile(0, &L_in); empile(0, &L_in); empile(2, &L_in);
    empile(7, &L_in); empile(3, &L_in);

    empile(1, &L_resultat); empile(2, &L_resultat); empile(0, &L_resultat); empile(0, &L_resultat); 
    empile(9, &L_resultat); empile(8, &L_resultat); empile(2, &L_resultat); empile(0, &L_resultat);
    empile(7, &L_resultat); empile(0, &L_resultat); empile(3, &L_resultat); empile(0, &L_resultat);

    Tic(&L_in);
    bool resultat = egale(L_in, L_resultat);
    VideListe(&L_in);
    VideListe(&L_resultat);

    return resultat;
}

bool test_Tic_vide() {
    /*[] -> []*/
    Liste L_in = NULL;
    Liste L_resultat = NULL;

    Tic(&L_in);
    return egale(L_in, L_resultat);
}

bool test_Tic_singleton() {
    /*[1] -> [0, 1]*/
    Liste L_in = NULL;
    Liste L_resultat = NULL;

    empile(1, &L_in);
    empile(1, &L_resultat); empile(0, &L_resultat); 

    Tic(&L_in);
    return egale(L_in, L_resultat);
}

bool test_Tic_zero() {
    /*[0] -> []*/
    Liste L_in = NULL;
    Liste L_resultat = NULL;

    empile(0, &L_in);

    Tic(&L_in);
    return egale(L_in, L_resultat);
}

bool test_Tic_deuxZero() { // On a obtenu un SegFault dans ce cas, c'est pourquoi on inclu ce test.
    /*[0, 0] -> []*/
    Liste L_in = NULL;
    Liste L_resultat = NULL;

    empile(0, &L_in); empile(0, &L_in);

    Tic(&L_in);
    return egale(L_in, L_resultat);
}

bool test_Tic_troisZero() {
    /*[0, 0, 0] -> []*/
    Liste L_in = NULL;
    Liste L_resultat = NULL;

    empile(0, &L_in); empile(0, &L_in); empile(0, &L_in);

    Tic(&L_in);
    return egale(L_in, L_resultat);
}

bool test_Tic_sansZero() {
    /*[1, 2, 3] -> [0, 1, 0, 2, 0, 3]*/
    Liste L_in = NULL;
    Liste L_resultat = NULL;

    empile(3, &L_in); empile(2, &L_in); empile(1, &L_in);

    empile(3, &L_resultat); empile(0, &L_resultat); empile(2, &L_resultat); empile(0, &L_resultat);
    empile(1, &L_resultat); empile(0, &L_resultat);

    Tic(&L_in);
    return egale(L_in, L_resultat);
}





/*************************************************/
/*                                               */
/*           Main                                */
/*                                               */
/*************************************************/


suite_tests cree_tests() {
    suite_tests tests_projet_2 = NULL;

    
    empile_test(&tests_projet_2, &test_Tic, "Tic: Complet");    
    empile_test(&tests_projet_2, &test_Tic_vide, "Tic: Vide");
    empile_test(&tests_projet_2, &test_Tic_sansZero, "Tic: Sans 0");    
    empile_test(&tests_projet_2, &test_Tic_troisZero, "Tic: Trois 0");
    empile_test(&tests_projet_2, &test_Tic_deuxZero, "Tic: Deux 0");
    empile_test(&tests_projet_2, &test_Tic_zero, "Tic: Singleton 0");
    empile_test(&tests_projet_2, &test_Tic_singleton, "Tic: Singleton 1");


    empile_test(&tests_projet_2, &test_AjouteDevantDernierZeroIter, "AjouteDevantDernierZeroIter: Complet");    
    empile_test(&tests_projet_2, &test_AjouteDevantDernierZeroIter_sansZero, "AjouteDevantDernierZeroIter: Sans 0");
    empile_test(&tests_projet_2, &test_AjouteDevantDernierZeroIter_vide, "AjouteDevantDernierZeroIter: Vide");
    empile_test(&tests_projet_2, &test_AjouteDevantDernierZeroIter_premierZero, "AjouteDevantDernierZeroIter: Premier est 0");

    empile_test(&tests_projet_2, &test_AjouteDevantDernierZeroRecTer, "AjouteDevantDernierZeroRecTer: Complet");    
    empile_test(&tests_projet_2, &test_AjouteDevantDernierZeroRecTer_sansZero, "AjouteDevantDernierZeroRecTer: Sans 0");
    empile_test(&tests_projet_2, &test_AjouteDevantDernierZeroRecTer_vide, "AjouteDevantDernierZeroRecTer: Vide");
    empile_test(&tests_projet_2, &test_AjouteDevantDernierZeroRecTer_premierZero, "AjouteDevantDernierZeroRecTer: Premier est 0");

    empile_test(&tests_projet_2, &test_AjouteDevantDernierZeroRec, "AjouteDevantDernierZeroRec: Complet");    
    empile_test(&tests_projet_2, &test_AjouteDevantDernierZeroRec_sansZero, "AjouteDevantDernierZeroRec: Sans 0");
    empile_test(&tests_projet_2, &test_AjouteDevantDernierZeroRec_vide, "AjouteDevantDernierZeroRec: Vide");
    empile_test(&tests_projet_2, &test_AjouteDevantDernierZeroRec_premierZero, "AjouteDevantDernierZeroRec: Premier est 0");


    empile_test(&tests_projet_2, &test_AjouteDevantPremierZero, "AjouteDevantPremierZero: Complet");    
    empile_test(&tests_projet_2, &test_AjouteDevantPremierZero_sansZero, "AjouteDevantPremierZero: Sans 0");
    empile_test(&tests_projet_2, &test_AjouteDevantPremierZero_vide, "AjouteDevantPremierZero: Vide");
    empile_test(&tests_projet_2, &test_AjouteDevantPremierZero_premierZero, "AjouteDevantPremierZero: Premier est 0");


    empile_test(&tests_projet_2, &test_FonctVireDernierIter, "FonctVireDernierIter: Complet");
    empile_test(&tests_projet_2, &test_FonctVireDernierIter_singleton, "FonctVireDernierIter: Singleton");
    empile_test(&tests_projet_2, &test_FonctVireDernierIter_vide, "FonctVireDernierIter: Vide");

    empile_test(&tests_projet_2, &test_FonctVireDernierRec, "FonctVireDernierRec: Complet");
    empile_test(&tests_projet_2, &test_FonctVireDernierRec_singleton, "FonctVireDernierRec: Singleton");
    empile_test(&tests_projet_2, &test_FonctVireDernierRec_vide, "FonctVireDernierRec: Vide");

    
    empile_test(&tests_projet_2, &test_NombreMemePositionIter, "NombreMemePositionIter: Complet");
    empile_test(&tests_projet_2, &test_NombreMemePositionIter_differentes_longueurs, "NombreMemePositionIter: Differentes longueurs");
    empile_test(&tests_projet_2, &test_NombreMemePositionIter_egales, "NombreMemePositionIter: Egales");
    empile_test(&tests_projet_2, &test_NombreMemePositionIter_deux_vides, "NombreMemePositionIter: Deux vides");
    empile_test(&tests_projet_2, &test_NombreMemePositionIter_un_vide, "NombreMemePositionIter: Une vide");

    empile_test(&tests_projet_2, &test_NombreMemePositionRec, "NombreMemePositionRec: Complet");
    empile_test(&tests_projet_2, &test_NombreMemePositionRec_differentes_longueurs, "NombreMemePositionRec: Differentes longueurs");
    empile_test(&tests_projet_2, &test_NombreMemePositionRec_egales, "NombreMemePositionRec: Egales");
    empile_test(&tests_projet_2, &test_NombreMemePositionRec_deux_vides, "NombreMemePositionRec: Deux vides");
    empile_test(&tests_projet_2, &test_NombreMemePositionRec_un_vide, "NombreMemePositionRec: Une vide");

    empile_test(&tests_projet_2, &test_NombreMemePositionRecTerFonc, "NombreMemePositionRecTerFonc: Complet");
    empile_test(&tests_projet_2, &test_NombreMemePositionRecTerFonc_differentes_longueurs, "NombreMemePositionRecTerFonc: Differentes longueurs");
    empile_test(&tests_projet_2, &test_NombreMemePositionRecTerFonc_egales, "NombreMemePositionRecTerFonc: Egales");
    empile_test(&tests_projet_2, &test_NombreMemePositionRecTerFonc_deux_vides, "NombreMemePositionRecTerFonc: Deux vides");
    empile_test(&tests_projet_2, &test_NombreMemePositionRecTerFonc_un_vide, "NombreMemePositionRecTerFonc: Une vide");

    empile_test(&tests_projet_2, &test_NombreMemePositionRecTerProc, "NombreMemePositionRecTerProc: Complet");
    empile_test(&tests_projet_2, &test_NombreMemePositionRecTerProc_differentes_longueurs, "NombreMemePositionRecTerProc: Differentes longueurs");
    empile_test(&tests_projet_2, &test_NombreMemePositionRecTerProc_egales, "NombreMemePositionRecTerProc: Egales");
    empile_test(&tests_projet_2, &test_NombreMemePositionRecTerProc_deux_vides, "NombreMemePositionRecTerProc: Deux vides");
    empile_test(&tests_projet_2, &test_NombreMemePositionRecTerProc_un_vide, "NombreMemePositionRecTerProc: Une vide");


    empile_test(&tests_projet_2, &test_Croissante, "Croissante: Complet");
    empile_test(&tests_projet_2, &test_Croissante_non_stricte, "Croissante: Non strictement croissante");
    empile_test(&tests_projet_2, &test_Croissante_singleton, "Croissante: Singleton");
    empile_test(&tests_projet_2, &test_Croissante_vide, "Croissante: Vide");
    
    
    empile_test(&tests_projet_2, &test_UnPlusDeuxEgalTrois, "UnPlusDeuxEgalTrois: Complet");
    empile_test(&tests_projet_2, &test_UnPlusDeuxEgalTrois_contreexemple, "UnPlusDeuxEgalTrois: Contre-exemple");
    empile_test(&tests_projet_2, &test_UnPlusDeuxEgalTrois_vide, "UnPlusDeuxEgalTrois: Vide");
    empile_test(&tests_projet_2, &test_UnPlusDeuxEgalTrois_couple, "UnPlusDeuxEgalTrois: Couple non-null");


    return tests_projet_2;
}


int main(int argc, char** argv)
{
    test_FonctVireDernierIter(); // Ici pour déboguer.
    suite_tests tests_projet_2 = cree_tests();
    lance_tests(tests_projet_2);
    libere_tests(tests_projet_2);
    return 0;
}