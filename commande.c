#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <assert.h>
#include <stdbool.h>
#include "noeud.h"
#include "auxiliare.h"
#include "commande.h"

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


void ls(noeud *courant){
    if (!courant->est_dossier) {
        printf("'%s' n'est pas un dossier.\n", courant->nom);
        return;
    }

    liste_noeud *l = courant->fils;

    while(l != NULL){
        printf("%s\n", l->noeud->nom);
        l = l->suiv;
    }

    free(l);
}

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

void print(noeud* courant, char tab[]){   // on commence avec la racine
    if (courant == NULL) return;

    int nb = nbFils(courant->fils);
    if (nb == 0) {
        printf("%sNoeud : '%s' (%s), %d fils\n", tab, courant->nom, courant->est_dossier ? "D" : "F", nb);
        return;
    }
    printf("%sNoeud : '%s' (%s), %d fils :", tab, courant->nom, courant->est_dossier ? "D" : "F", nb);

    if (courant->fils != NULL){
        liste_noeud* l = courant->fils;
        while (l != NULL){
            printf(" %s (%s),", l->noeud->nom, l->noeud->est_dossier ? "D" : "F");
            l = l->suiv;
        }
        printf("\n");
        l = courant->fils;
        char tab2[100] = "";
        strcat(tab2, tab);
        strcat(tab2, "|     ");
        while (l != NULL){
            print(l->noeud, tab2);
            l = l->suiv;
        }
    }
}

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
    printf("/");
    for (int i = n-1; i >= 0; i--){
        printf("%s/", pwd[i]);
    }
    printf("\n");
    free(pwd);
}

noeud* cd(noeud* courant, char* chemin){
    assert(courant != NULL);
    assert(courant->est_dossier);

    if (chemin[0] == '\0') return courant->racine;
    if (chemin[0] == '/') return cd(courant->racine, chemin+1);
    if (chemin[0] == '.'){
        if (chemin[1] == '.'){
            if (chemin[2] == '\0') return courant->pere;
            if (chemin[2] == '/') return cd(courant->pere, chemin+3);
        }
        if (chemin[1] == '/') 
            return cd(courant, chemin+2);
    }

    char* reste = strchr(chemin, '/');
    reste = (reste == NULL) ? NULL : reste+1;
    liste_noeud* liste = courant->fils;

    if (reste == NULL){
        while (liste != NULL){
            if (strcmp(liste->noeud->nom, chemin) == 0){
                if (liste->noeud->est_dossier) return liste->noeud;
                else {
                    printf("'%s' n'est pas un dossier. Le dossier courant reste à la même place.\n", liste->noeud->nom);
                    return NULL;
                }
            }
            liste = liste->suiv;
        }
        return NULL;
    }

    int len = strlen(chemin) - (strlen(reste)+1); 
    char* premier_mot = malloc(sizeof(char) * (len + 1));
    assert(premier_mot != NULL);
    memmove(premier_mot, chemin, sizeof(char) * len);
    premier_mot[len] = '\0';

    while(liste != NULL){
        if (strcmp(liste->noeud->nom, premier_mot) == 0){
            free(premier_mot);
            return cd(liste->noeud, reste);
        }
        liste = liste->suiv;
    }

    return NULL;
}

void rm(noeud* courant, char* chemin, noeud* origin){
    assert(courant != NULL);

    if (chemin[0] == '\0'){
        printf("Erreur, il faut passer un chemin non vide en parametre.\n");
        return;
    }
    if (chemin[0] == '/') {rm(courant->racine, chemin+1, origin); return;}
    if (chemin[0] == '.'){
        if (chemin[1] == '.' && chemin[2] == '/') {rm(courant->pere, chemin+3, origin); return;}
        if (chemin[1] == '/') {rm(courant, chemin+2, origin); return;}
    }

    char* reste = strchr(chemin, '/');
    reste = (reste == NULL) ? NULL : reste+1;
    liste_noeud* liste = courant->fils;

    if (reste == NULL){  // il y a plus d'occurence de '/' dans chemin
        while (liste != NULL){
            if (strcmp(liste->noeud->nom, chemin) == 0){    // on a trouvé le noeud à supprimer  
                while(origin != courant->racine){
                    if (strcmp(origin->nom, liste->noeud->nom) == 0){
                        printf("Le dossier '%s' ne peut pas être supprimé car elle est un parent de '%s'\n", liste->noeud->nom, origin->nom);
                        return;
                    }
                    origin = origin->pere;
                }
                if (liste->noeud->est_dossier) freeFils(liste->noeud);
                else free(liste->noeud); 
                       
                liste_noeud* tmp = courant->fils;
                if (tmp == liste){
                    courant->fils = liste->suiv;
                    return;
                }
                while (tmp->suiv != liste) tmp = tmp->suiv;
                tmp->suiv = liste->suiv;
                return;
            }
            liste = liste->suiv;
        }
        printf("'%s' n'existe pas dans le dossier. Il ne peut pas être supprimé.\n", chemin);
        return;
    }

    int len = strlen(chemin) - (strlen(reste)+1); 
    char* premier_mot = malloc(sizeof(char)*len);
    assert(premier_mot != NULL);
    memmove(premier_mot, chemin, sizeof(char)*len);

    while(liste != NULL){
        if (strcmp(liste->noeud->nom, premier_mot) == 0){
            rm(liste->noeud, reste, origin);
            return;
        }
        liste = liste->suiv;
    }

    printf("'%s' n'existe pas dans le dossier. Il ne peut pas être supprimé.\n", premier_mot);
    return;
}



void cp(noeud* courant, char* chem1, char* chem2) {
    assert(courant != NULL);

    noeud *depart = cd(courant, chem1);

    if(depart == NULL){
        printf("Erreur, destination indiqué par chemin1 est éroné.%s", chem1);
        return;
    }

    char *noLast = withoutLastName(chem2);
    char *last = getLastName(chem2);
    
    noeud *copie = copier_noeud(depart);
    for (int i = 0; i < strlen(last); i++) copie->nom[i] = last[i];
    copie->nom[strlen(last)] = '\0';

    if (strchr(chem2, '/') == NULL){
        copie->pere = courant;
        if (!appartient_sous_arbre(courant, courant->fils)){
            ajout_noeud_a_liste(copie, &courant->fils);
            return;
        }else{  
            puts("Il a apparait deja dans le noeud courant");
            return;
        }
    }

    copie->pere = courant;
    noeud *arrive = cd(courant, noLast);

    if (arrive == NULL){
        printf("Erreur, l'endroit où on souhaite copier le premier element indiqué par le chemin2 est un chemin menant nul part.");
        return;
    }
    if (!arrive->est_dossier){
        printf("Erreur, l'endroit où on souhaite copier le premier element indiqué par le chemin1 est un fichier.");
        return;
    }
    if (estParent(depart, arrive)){
        printf("Erreur, l'endroit où on souhaite copier le premier element indiqué par le chemin1 est un parent du chemin2.");
        return;
    }
    if (appartient_sous_arbre(copie, arrive->fils)){
        printf("Erreur, l'endroit où on souhaite copier le premier element indiqué par le chemin1 est un parent du chemin2.");
        return;
    }

    ajout_noeud_a_liste(copie, &arrive->fils);

    free(noLast);   
}

void mv(noeud *courant, char *chemin1, char *chemin2) {
    noeud *depart = cd(courant, chemin1);
    if (depart ==NULL) {
        printf("Erreur, destination indiqué par chemin1 est éroné.%s", chemin1);
        return;
    }

    char *noLast = withoutLastName(chemin2);
    char *last = getLastName(chemin2);

    if (strchr(chemin2, '/') == NULL){
        memcpy(depart->nom, last, sizeof(char) * strlen(last));
        depart->nom[strlen(last)] = '\0';
        return;
    }

    noeud *arrive= cd(courant,noLast);

    if (arrive == NULL) {
        printf("Erreur, l'endroit où on souhaite déplacer le premier élément indiqué par le chemin2 est un chemin menant nulle part.");
        return;
    }
    if (!arrive->est_dossier) {
        printf("Erreur, l'endroit où on souhaite déplacer le premier élément indiqué par le chemin1 est un fichier.");
        return;
    }
    if (estParent(depart, arrive)) {
        printf("Erreur, l'endroit où on souhaite déplacer le premier élément indiqué par le chemin1 est un parent du chemin2 .");
        return;
    }
    if (appartient_sous_arbre(depart, arrive->fils)) {
        printf("Erreur, l'endroit où on souhaite déplacer le premier élément indiqué par le chemin1 est un parent du chemin2.");
        return;
    }

    liste_noeud *liste = depart->pere->fils;
    if (liste->noeud ==depart) {
        depart->pere->fils = liste->suiv;
    } else {
        while (liste != NULL && liste->suiv != NULL && liste->suiv->noeud != depart) {
            liste = liste->suiv;
        }
        if (liste != NULL&& liste->suiv != NULL) {
            liste_noeud *tmp = liste->suiv;
            liste->suiv = tmp->suiv;
        }
    }

    memcpy(depart->nom, last, sizeof(char) * strlen(last));
    depart->nom[strlen(last)] = '\0';
    depart->pere = arrive;
    ajout_noeud_a_liste(depart, &arrive->fils);

    free(noLast);
}

