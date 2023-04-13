#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>
#include <assert.h>
#include <ctype.h>
#include "noeud.h"

void freeFils(noeud* courant){
    if (courant->fils == NULL) return;
    liste_noeud* l = courant->fils;
    while (l != NULL){
        freeFils(l->noeud);
        l = l->suiv;
    }
    free(l);
    free(courant->fils);
}

// recherche le fils d'un noeud avec un nom donné
noeud* trouve_fils(noeud* courant, char* nom) {
    if (!courant->est_dossier) {
        return NULL;
    }
    liste_noeud* l = courant->fils;
    while (l != NULL){
        if (strcmp(l->noeud->nom, nom) == 0) {
            return l->noeud;
        }
        l = l->suiv;
    }
    return NULL;
}

int nbFils(liste_noeud* liste){
    if (liste == NULL) return 0;
    liste_noeud* l = liste;
    int n = 0;
    while (l != NULL){
        n++;
        l = l->suiv;
    } 
    return n;
}

// verifie si noeud appartient au sous arbre
bool appartient_sous_arbre(noeud* noeud, liste_noeud* arbre) {
    if (arbre == NULL) {
        return false;
    }
    while (arbre != NULL) {
        if (noeud == arbre->noeud) {
            return true;
        }
        if (appartient_sous_arbre(noeud,arbre->noeud->fils)) {
            return true;
        }
        arbre=arbre->suiv;
    }
    return false;
}

// Ajoute un noeud fils à un noeud
void ajouter_fils(noeud *pere, noeud *fils) {
    liste_noeud *l = pere->fils;
    while (l != NULL) {
        if (strcmp(l->noeud->nom,fils->nom) == 0) {
            printf("Le noeud %s existe déjà dans le dossier %s.\n", fils->nom, pere->nom);
            return;
        }
        l = l->suiv;
    }
    
    liste_noeud *nvfils = malloc(sizeof(liste_noeud));
    nvfils->noeud= fils;
    nvfils->suiv = NULL;
    
    if (pere->fils== NULL) {
        pere->fils =nvfils;
    } else {
        liste_noeud *dernier_fils = pere->fils;
        while (dernier_fils->suiv != NULL) {
            dernier_fils = dernier_fils->suiv;
        }
        dernier_fils->suiv = nvfils;
    }

    fils->pere = pere;
}

// copie un noeud et son sous-arbre
noeud* copier_noeud(noeud* n) {
    noeud* cp = (noeud*) malloc(sizeof(noeud));
    cp ->est_dossier = n->est_dossier;
    strcpy(cp ->nom, n->nom);
    cp->pere = NULL;
    cp->racine = NULL;
    cp ->fils = NULL;

    liste_noeud* fils = n->fils;
    while (fils != NULL) {
        ajouter_fils(cp , copier_noeud(fils->noeud));
        fils = fils->suiv;
    }

    return cp;
}