#include <stdio.h>
#include <stdlib.h>

#define AND &&
#define OR ||
#define ISNOT !=
#define NOT !
#define then

typedef enum { FALSE, TRUE} bool;


/*************************************************/
/*                                               */
/*           ListesZ                             */
/*                                               */
/*************************************************/



typedef struct BlocZ {
    int valeur;
    struct BlocZ *next;
    struct BlocZ *prev;
} BlocZ;


void ZAjouteAvant(BlocZ** acces, int x) {
    // On peut omettre cette ligne, et dans tel cas on obtient un SegFault si acces est NULL.
    if(acces == NULL) return; 

    BlocZ *tmp = (BlocZ*)malloc(sizeof(BlocZ));
    tmp -> valeur = x;

    if (*acces == NULL) {
        tmp -> next = tmp;
        tmp -> prev = tmp;
        *acces = tmp;
        return;
    }

    tmp -> next = (*acces) -> next;
    (*acces) -> next = tmp;
    tmp -> prev = (*acces);
    *acces = tmp;
}

bool test_ZAjouteAvant_vide() {
    BlocZ *b = NULL;
    BlocZ **acces = &b;
    ZAjouteAvant(acces, 1);
    return (*acces) -> valeur == 1 &&
    (*acces) -> next == *acces &&
    (*acces) -> prev == *acces;
}

bool test_ZAjouteAvant() {
    BlocZ *b1 = (BlocZ*)malloc(sizeof(BlocZ));
    BlocZ *b2 = (BlocZ*)malloc(sizeof(BlocZ));
    BlocZ *b3 = (BlocZ*)malloc(sizeof(BlocZ));

    b1 -> valeur = 1;
    b1 -> prev = b3;
    b1 -> next = b2;

    b2 -> valeur = 2;
    b2 -> prev = b1;
    b2 -> next = b3;

    b3 -> valeur = 3;
    b3 -> prev = b2;
    b3 -> next = b1;

    BlocZ** acces = &(b1->prev);

    ZAjouteAvant(acces, 4);
    
    bool resultat = (*acces) -> valeur == 4 &&
        (*acces) -> next == b1 &&
        (*acces) -> prev == b3;
    
    free(*acces);
    free(b1);
    free(b2);
    free(b3);
    return resultat;

}

int main() {
    printf("TESTS LISTEZ\n");
    printf("-------------------------\n");

    printf("Test Complet: ");
    if(test_ZAjouteAvant()) {
        printf("Reussite!");
    }
    else {
        printf("Echec!");
    }
    printf("\n");

    printf("Test Vide: ");
    if(test_ZAjouteAvant_vide()) {
        printf("Reussite!");
    }
    else {
        printf("Echec!");
    }

    return 0;
}