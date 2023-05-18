#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>
#include <assert.h>
#include <ctype.h>
#include "noeud.h"

void freeFils(noeud *n){
    if (n == NULL) return;
    if (n->fils != NULL) {
        liste_noeud *tmp = n->fils;
        while (tmp != NULL) {
            liste_noeud *suiv = tmp->suiv;
            freeFils(tmp->noeud);
            free(tmp);
            tmp = suiv;
        }
    }
    free(n);
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
    assert(pere->est_dossier);

    if (pere->fils == NULL){
        liste_noeud *l = malloc(sizeof(liste_noeud));
        assert(l != NULL);
        l->noeud = fils;
        l->suiv = NULL;
        pere->fils = l;
    }else{
        ajout_noeud_a_liste(fils, &(pere->fils));
    }
}

// copie un noeud et son sous-arbre
noeud* copier_noeud(noeud* n) {
    noeud* cp = malloc(sizeof(noeud));
    assert(cp != NULL);
    strcpy(cp ->nom, n->nom);
    cp->est_dossier = n->est_dossier;
    cp->pere = n->pere;    
    cp->racine = n->racine;
    cp->fils = NULL;

    liste_noeud* fils = n->fils;
    while (fils != NULL) {
        ajouter_fils(cp , copier_noeud(fils->noeud));
        fils = fils->suiv;
    }

    return cp;
}

void freeListeNoeud(liste_noeud* list) {
    if (list == NULL) {
        return;
    }
    freeListeNoeud(list->suiv);
    free(list);
}


def quicksort(array):
    if len(array) <= 1:
        return array

    pivot = array[len(array)//2]
    left = [x for x in array if x < pivot]
    middle = [x for x in array if x == pivot]
    right = [x for x in array if x > pivot]

    return quicksort(left) + middle + quicksort(right)
