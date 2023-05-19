#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <assert.h>
#include <stdbool.h>
#include "noeud.h"
#include "auxiliare.h"
#include "commande.h"

/**
 * @brief Crée un dossier.
 *
 * Cette fonction prend le noeud et un char[] en entrée et crée un dossier.
 *
 * @param courant Le noeud courant.
 * @param nom Le nom du dossier.
 */
void mkdir(noeud *courant, char* nom){
    if (strlen(nom) >= 100){
        printf("Le nom du dossier est trop long.\n");
        return;
    }

    if (courant->est_dossier == false) return;
    noeud *n = malloc(sizeof(noeud));
    assert(n != NULL);
    n->est_dossier = true;
    estAlpha(nom);
    memcpy(n->nom, nom, sizeof(char) * (strlen(nom) + 1));
    n->pere = courant;
    n->racine = courant->racine;
    n->fils = NULL;

    liste_noeud *l = malloc(sizeof(liste_noeud));
    assert(l != NULL);

    if (courant->fils == NULL){
        courant->fils = l;
        l->noeud = n;
        l->suiv = NULL;
    }else{
        liste_noeud *tmp = courant->fils;
        while (tmp->suiv != NULL) tmp = tmp->suiv;
        tmp->suiv = l;
        l->noeud = n;
        l->suiv = NULL;
    }
}

/**
 * @brief Affiche les fichiers et dossiers.
 *
 * Cette fonction prend le noeud en entrée et affiche les fichiers et dossiers.
 *
 * @param courant Le noeud courant.
 */
void ls(noeud *courant){
    if (!courant->est_dossier) {
        printf("'%s' n'est pas un dossier.\n", courant->nom);
        return;
    }
    liste_noeud *l = courant->fils;

    while(l != NULL){
        printf("%s ", l->noeud->nom);
        l = l->suiv;
    }
    printf("\n");
    free(l);
}

/**
 * @brief Crée un fichier.
 *
 * Cette fonction prend le noeud et un char[] en entrée et crée un fichier.
 *
 * @param courant Le noeud courant.
 * @param nom Le nom du fichier.
 */
void touch(noeud* courant, char* nom){
    assert(courant->est_dossier);

    noeud *n = malloc(sizeof(noeud));
    assert(n != NULL);
    estAlpha(nom);

    memcpy(n->nom, nom, sizeof(char) * (strlen(nom) + 1)); // Ajout de +1 ici
    n->est_dossier = false;
    n->pere = courant;
    n->racine = courant->racine;
    n->fils = NULL;

    liste_noeud *l = malloc(sizeof(liste_noeud));
    assert(l != NULL);

    if (courant->fils == NULL){
        courant->fils = l;
        l->noeud = n;
        l->suiv = NULL;
    }else{
        liste_noeud *tmp = courant->fils;
        while (tmp->suiv != NULL) tmp = tmp->suiv;
        tmp->suiv = l;
        l->noeud = n;
        l->suiv = NULL;
    }

}

/**
 * @brief Imprime l'arborescence.
 *
 * Cette fonction prend le noeud et un char[] en entrée et imprime l'arbre.
 *
 * @param courant Le noeud courant.
 * @param tab Le tableau de char[] représenté par "|      ".
 */
void print(noeud* courant, char tab[]){   // on commence avec la racine
    if (courant == NULL) return;

    int nb = nbFils(courant->fils);
    if (nb == 0) {
        printf("%sNoeud : '%s' (%s), 0 fils\n", tab, courant->nom, (courant->est_dossier) ? "D" : "F");
        return;
    }
    printf("%sNoeud : '%s' (%s), %d fils :", tab, courant->nom, (courant->est_dossier) ? "D" : "F", nb);

    liste_noeud* l = courant->fils;
    while (l != NULL){
        printf(" %s (%s),", l->noeud->nom, (l->noeud->est_dossier) ? "D" : "F");
        l = l->suiv;
    }
    printf("\n");

    l = courant->fils;
    char tab2[1000] = "";       // --> On suppose qu'on a pas plus de 1000 hauteurs
    strcat(tab2, tab);
    strcat(tab2, "|     ");

    while (l != NULL){
        print(l->noeud, tab2);
        l = l->suiv;
    }
}

/**
 * @brief Affiche le chemin du noeud courant.
 *
 * Cette fonction prend le noeud en entrée et affiche le chemin du noeud courant.
 *
 * @param courant Le noeud courant.
 */
void pwd(noeud* courant){
    assert(courant != NULL);
    noeud* d = courant;
    int n = 0;
    while(d != courant->racine){
        n++;
        d = d->pere;
    }
    d = courant;
    char** pwd = (char**) malloc(sizeof(char*)*n);
    assert(pwd != NULL);
    for (int i = 0; i < n; i++){
        pwd[i] = d->nom;
        d = d->pere;
    }
    
    for (int i = n-1; i >= 0; i--){
        printf("/%s", pwd[i]);
    }
    printf("\n");
    free(pwd);
}

/**
 * @brief Change le noeud courant.
 *
 * Cette fonction prend le noeud et un char * en entrée et change le noeud courant.
 *
 * @param courant Le noeud courant.
 * @param chemin Le chemin de la destination de noeud courant.
 * @return Le nouvel emplacement de noeud courant.
 */
noeud* cd(noeud* courant, char* chemin){
    assert(courant != NULL);
    assert(courant->est_dossier);

    noeud *arrive = destination(courant, chemin);
    if (arrive == NULL) {
        printf("Erreur, le chemin indiqué est éroné.\n");
        return NULL;
    }
    if (!arrive->est_dossier) {
        printf("Erreur, le chemin indiqué est un fichier.\n");
        return NULL;
    }
    return arrive;
}

/**
 * @brief Supprime un noeud.
 *
 * Cette fonction prend le noeud, un char * et le noeud courant en entrée et supprime un noeud.
 * Si le noeud est un dossier, on supprime tous les fils de ce dossier.
 * Si le noeud est un fichier, on supprime le fichier.
 * Si le noeud est un parent du noeud courant, on ne peut pas le supprimer.
 * Si le noeud n'existe pas, on ne peut pas le supprimer.
 *
 * @param courant Le noeud courant.
 * @param chemin Le chemin du noeud à supprimer.
 * @param origin Le noeud courant.
 */
void rm(noeud* courant, char* chemin){
    noeud *dest = destination(courant, chemin);

    if (dest == NULL) {
        printf("Erreur, le chemin indiqué est éroné.\n");
        return;
    }
    if (estParent(dest, courant)) {
        printf("Erreur, le chemin indiqué est un parent du noeud courant.\n");
        return;
    }

    liste_noeud *liste = dest->pere->fils;

    if (strcmp(liste->noeud->nom, dest->nom) == 0) {
        dest->pere->fils = liste->suiv;
        freeNoeud(liste->noeud);
        free(liste);    
        return;
    }
    while (liste->suiv != NULL && strcmp(liste->suiv->noeud->nom, dest->nom) != 0) {
        liste = liste->suiv;
    }
    if (liste->suiv != NULL) {
        liste_noeud *tmp = liste->suiv;
        liste->suiv = tmp->suiv;
        freeNoeud(tmp->noeud);
        free(tmp);
        return;
    }
    printf("'%s' n'existe pas dans le dossier. Il ne peut pas être supprimé.\n", chemin);
    return;
}

/**
 * @brief Copie un noeud.
 *
 * Cette fonction prend le noeud, deux char * et le noeud courant en entrée et copie un noeud.
 *
 * @param courant Le noeud courant.
 * @param chem1 Le chemin du noeud à copier.
 * @param chem2 Le chemin de la destination du noeud copié.
 */
void cp(noeud* courant, char* chem1, char* chem2) {
    assert(courant != NULL);

    noeud *depart = destination(courant, chem1);

    if(depart == NULL){
        printf("Erreur, destination indiqué par chemin1 (%s) est éroné.", chem1);
        return;
    }

    char *noLast = withoutLastName(chem2);
    char *last = getLastName(chem2);
    
    noeud *copie = copier_noeud(depart);
    strncpy(copie->nom, last, sizeof(char) * strlen(last));
    copie->nom[strlen(last)] = '\0';

    if (strchr(chem2, '/') == NULL){
        copie->pere = courant;
        if (!appartient_sous_arbre(courant, courant->fils) && !estParent(depart, courant)){
            ajout_noeud_a_liste(copie, &courant->fils);
        }else{
            fprintf(stderr, "Erreur, le noeud courant est un parent du noeud à copier.\n");
        }
        // free(noLast);
        // free(last);
        return;
    }

    copie->pere = courant;
    noeud *arrive = cd(courant, noLast);

    if (arrive == NULL){
        printf("Erreur, l'endroit où on souhaite copier le premier element indiqué par le chemin2 est un chemin menant nul part.");
        return;
    }
    if (!arrive->est_dossier){
        printf("Erreur, l'endroit où on souhaite copier le premier élément indiqué par le chemin1 est un fichier.");
        return;
    }
    if (estParent(depart, arrive)){
        printf("Erreur, l'endroit où on souhaite copier le premier élément indiqué par le chemin1 est un parent du chemin2.");
        return;
    }
    if (appartient_sous_arbre(copie, arrive->fils)){
        printf("Erreur, l'endroit où on souhaite copier le premier élément indiqué par le chemin1 est un parent du chemin2.");
        return;
    }

    ajout_noeud_a_liste(copie, &arrive->fils);

    free(noLast);
    free(last);
}

/**
 * @brief Déplace un noeud.
 *
 * Cette fonction prend le noeud, deux char * et le noeud courant en entrée et déplace un noeud.
 *
 * @param courant Le noeud courant.
 * @param chemin1 Le chemin du noeud à déplacer.
 * @param chemin2 Le chemin de la destination du noeud déplacé.
 */
void mv(noeud *courant, char *chemin1, char *chemin2) {
    noeud *depart = destination(courant, chemin1);

    if (depart ==NULL) {
        printf("Erreur, destination indiqué par chemin1 (%s) est éroné.", chemin1);
        return;
    }

    char *noLast = withoutLastName(chemin2);
    char *last = getLastName(chemin2);

    if (strchr(chemin2, '/') == NULL){
        strncpy(depart->nom, last, sizeof(depart->nom) - 1);
        depart->nom[sizeof(depart->nom) - 1] = '\0';
        free(noLast);
        free(last);
        return;
    }

    noeud *arrive= cd(courant, noLast);

    if (arrive == NULL) {
        printf("Erreur, l'endroit où l'on souhaite déplacer le premier élément indiqué par le chemin2 est un chemin menant nulle part.");
        free(noLast);
        free(last);
        return;
    }
    if (!arrive->est_dossier) {
        printf("Erreur, l'endroit où on souhaite déplacer le premier élément indiqué par le chemin1 est un fichier.");
        free(noLast);
        free(last);
        return;
    }
    if (estParent(depart, arrive)) {
        printf("Erreur, l'endroit où on souhaite déplacer le premier élément indiqué par le chemin1 est un parent du chemin2 .");
        free(noLast);
        free(last);
        return;
    }
    if (appartient_sous_arbre(depart, arrive->fils)) {
        printf("Erreur, l'endroit où on souhaite déplacer le premier élément indiqué par le chemin1 est un parent du chemin2.");
        free(noLast);
        free(last);
        return;
    }

    liste_noeud *liste = depart->pere->fils;
    if (liste->noeud == depart) {
        depart->pere->fils = liste->suiv;
        free(liste);
    } else {
        while (liste != NULL && liste->suiv != NULL && liste->suiv->noeud != depart) {
            liste = liste->suiv;
        }
        if (liste != NULL && liste->suiv != NULL) {
            liste_noeud *tmp = liste->suiv;
            liste->suiv = tmp->suiv;
            free(tmp);
        }
    }

    strncpy(depart->nom, last, sizeof(depart->nom) - 1);
    depart->nom[sizeof(depart->nom) - 1] = '\0';
    depart->pere = arrive;
    ajout_noeud_a_liste(depart, &arrive->fils);

    free(noLast);
    free(last);
}

