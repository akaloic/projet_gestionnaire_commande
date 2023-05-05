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
    if (arbre == NULL) return false;
    liste_noeud* l = arbre;
    while (l != NULL){
        if (strcmp(l->noeud->nom, noeud->nom) == 0) return true;
        l = l->suiv;
    }
    return false;
}

bool estParent(noeud *pere, noeud *fils){
    noeud *verifSA = fils;
    while(verifSA != verifSA->racine){
        if (verifSA == pere) return true;
        verifSA = verifSA->pere;
    }
    return false;
}

// Ajoute N à Fils
void ajout_noeud_a_liste(noeud *n, liste_noeud **fils){
    liste_noeud *tmp = (*fils);
    while(tmp->suiv != NULL) tmp = tmp->suiv;
    liste_noeud *nv = malloc(sizeof(liste_noeud));
    nv->noeud = n;
    nv->suiv = NULL;
    tmp->suiv = nv;
}

// Ajoute un noeud fils à un noeud
void ajouter_fils(noeud *pere, noeud *fils) {
    liste_noeud *nvfils = malloc(sizeof(liste_noeud));
    assert(nvfils != NULL);
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
    noeud* cp = malloc(sizeof(noeud));
    cp ->est_dossier = n->est_dossier;
    strcpy(cp ->nom, n->nom);
    cp->pere = n->pere;    // le pere est défini dans cp
    cp->racine = n->racine;
    cp ->fils = NULL;

    liste_noeud* fils = n->fils;
    while (fils != NULL) {
        ajouter_fils(cp , copier_noeud(fils->noeud));
        fils = fils->suiv;
    }

    return cp;
}