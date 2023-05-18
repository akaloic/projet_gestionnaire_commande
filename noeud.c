#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>
#include <assert.h>
#include <ctype.h>
#include "noeud.h"

/**
 * @brief Free un noeud et ses fils
 * 
 * @param n noeud dont on veut libérer les fils
 */
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


/**
 * @brief Trouve un fils dans un noeud
 * 
 * parcours la liste des fils du noeud courant et cherche le fils dont le nom est passé en paramètre
 * 
 * @param courant noeud dans lequel on cherche le fils
 * @param nom nom du fils
 * @return retourne le fils si il existe, NULL sinon
 */
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

/**
 * @brief Compte le nombre de fils d'un noeud
 * 
 * parcours la liste des fils du noeud et compte le nombre de fils
 * 
 * @param liste liste des fils du noeud
 * @return retourne le nombre de fils
 */
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

/**
 * @brief Vérifie si un noeud appartient à un sous arbre
 * 
 * parcours la liste des noeuds du sous arbre et vérifie si le noeud passé 
 * en paramètre appartient au sous arbre
 * 
 * @param noeud noeud à vérifier
 * @param arbre sous arbre dans lequel on veut vérifier
 * @return retourne true si le noeud appartient au sous arbre, false sinon
 */
bool appartient_sous_arbre(noeud* noeud, liste_noeud* arbre) {
    if (arbre == NULL) return false;
    liste_noeud* l = arbre;
    while (l != NULL){
        if (strcmp(l->noeud->nom, noeud->nom) == 0) return true;
        l = l->suiv;
    }
    return false;
}

/**
 * @brief Vérifie si un noeud est le parent d'un autre noeud
 * 
 * parcours tous les pères de x, et vérifie si le noeud parent est le noeud passé en paramètre (parent)
 * 
 * @param parent noeud sur lequel on veut vérifier si il est le parent du noeud x
 * @param x noeud x, à vérifier si il est le fils du noeud parent
 * @return retourne true si le noeud est le parent du noeud passé en paramètre, false sinon
 */
bool estParent(noeud *parent, noeud *x){
    noeud *verifSA = x;
    while(verifSA != verifSA->racine){
        if (verifSA == parent) return true;
        verifSA = verifSA->pere;
    }
    return false;
}

/**
 * @brief Ajoute un noeud à une liste de noeud
 * 
 * parcours la liste de noeud et ajoute le noeud passé en paramètre à la fin de la liste
 * 
 * @param n noeud à ajouter
 * @param fils liste de noeud dans laquelle on veut ajouter le noeud
 */
void ajout_noeud_a_liste(noeud *n, liste_noeud **fils) {
    if (*fils == NULL) {
        *fils = malloc(sizeof(liste_noeud));
        assert(*fils != NULL);
        (*fils)->noeud = n;
        (*fils)->suiv = NULL;
        return;
    }

    liste_noeud *l = malloc(sizeof(liste_noeud));
    assert(l != NULL);

    l->noeud = n;
    l->suiv = NULL;
    liste_noeud *tmp = *fils;

    while (tmp->suiv != NULL) {
        tmp = tmp->suiv;
    }
    
    tmp->suiv = l;
}

/**
 * @brief Ajoute un fils à un noeud
 * 
 * ajoute le fils passé en paramètre à la liste des fils du noeud père
 * 
 * @param pere noeud père
 * @param fils noeud fils
 */
void ajouter_fils(noeud *pere, noeud *fils) {
    assert(pere->est_dossier);

    if (pere->fils == NULL){
        liste_noeud *l = malloc(sizeof(liste_noeud));
        assert(l != NULL);
        l->noeud = fils;
        l->suiv = NULL;
        pere->fils = l;
    }else{
        ajout_noeud_a_liste(fils, &pere->fils);
    }
}

/**
 * @brief Copie un noeud
 * 
 * @param n noeud à copier
 * @return retourne le noeud du noeud passer en paramètre
 */
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