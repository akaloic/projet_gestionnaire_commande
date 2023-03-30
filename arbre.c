#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>
#include <assert.h>

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

void verifNoeud(noeud *n){
    if (n == NULL){
        printf("allocation null");
        assert(n != NULL);
    }
}

void verifListe(liste_noeud *n){
    if (n == NULL){
        printf("allocation null");
        assert(n != NULL);
    }
}

void mkdir(noeud *courant, char* nom){
    if (courant->est_dossier == false) return;
    noeud *n = malloc(sizeof(noeud));
    verifNoeud(n);
    n->est_dossier = true;
    memcpy(n->nom, nom, sizeof(char) * strlen(nom));
    n->pere = courant;
    n->racine = courant->racine;
    n->fils = NULL;

    liste_noeud *l = malloc(sizeof(liste_noeud));
    verifListe(l);
    l->noeud = n;
    l->suiv = courant->fils;
    courant->fils = l;
}

void ls(noeud *courant){
}

int main(){
    noeud *racine = malloc(sizeof(noeud));
    verifNoeud(racine);
    racine->est_dossier = true;
    memcpy(racine->nom, "/", sizeof(char) * 1);
    racine->pere = NULL;
    racine->racine = racine;
    racine->fils = NULL;

    mkdir(racine, "dossier1");
    mkdir(racine, "dossier2");
    mkdir(racine, "dossier3");

    ls(racine);

    free(racine);

    return EXIT_SUCCESS;    
}