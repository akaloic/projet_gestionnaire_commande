#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>

struct noeud;
struct liste_noeud;

struct noeud{
    bool est_dossier;
    char nom[100];
    struct noeud *pere; 
    struct noeud *racine;
    struct liste_noeud *fils;
};

struct liste_noeud{
    struct noeud *noeud;
    struct liste_noeud *suiv;
};

typedef struct noeud noeud;
typedef struct liste_noeud liste_noeud;

mkdir(noeud *courant, char* nom){
    assert(courant->est_dossier);

    noeud *n = malloc(sizeof(noeud));
    n->est_dossier = true,
    memmove(n->nom, nom, sizeof(char)*strlen(nom));
    memmove(n->pere, courant, sizeof(noeud));
    memmove(n->racine, courant->racine, sizeof(noeud));
    //pas besoin d'initié fils, revient à le mettre à NULL

    //je finirai plus tard
}

int main(int nbr, char *argv){
    return EXIT_SUCCESS;
}