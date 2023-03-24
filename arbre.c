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

void mkdir(noeud *courant, char* nom){
    noeud *n = malloc(sizeof(noeud));
    n->est_dossier = true,
    memcpy(n, nom, sizeof(char) * strlen(nom));
    memmove(n->pere, courant, sizeof(noeud));
    memmove(n->racine, courant->racine, sizeof(noeud));
    memmove(n->fils, NULL, sizeof(liste_noeud));

    if (courant->fils == NULL) memmove(courant->fils, n, sizeof(liste_noeud));
    else{
        liste_noeud *l = courant->fils;
        while (l->suiv != NULL) l = l->suiv;
        l->suiv = malloc(sizeof(liste_noeud));
        memmove(l->suiv->noeud, n, sizeof(noeud));
        memmove(l->suiv->suiv, NULL, sizeof(liste_noeud));
    }

    free(n);
}

void ls(noeud *courant){
    if (courant->fils == NULL) return;
    liste_noeud *l = courant->fils;
    while (l != NULL){
        printf("%s ", l->noeud->nom);
        l = l->suiv;
    }
}

int main(){
    noeud *racine = malloc(sizeof(noeud));
    racine->est_dossier = true;
    memcpy(racine->nom, "/", sizeof(char) * strlen("/"));
    memmove(racine->pere, NULL, sizeof(noeud));
    memmove(racine->racine, racine, sizeof(noeud));
    memmove(racine->fils, NULL, sizeof(liste_noeud));

    mkdir(racine, "home");
    mkdir(racine, "usr");
    mkdir(racine, "bin");

    //ls(racine);

    return EXIT_SUCCESS;
}