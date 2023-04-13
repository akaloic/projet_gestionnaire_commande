#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <assert.h>
#include <stdbool.h>
#include "noeud.h"
#include "auxiliare.h"
#include "commande.h"

void mkdir(noeud *courant, char* nom){
    if (courant->est_dossier == false) return;
    noeud *n = malloc(sizeof(noeud));
    assert(n != NULL);
    n->est_dossier = true;
    estAlpha(nom);
    memcpy(n->nom, nom, sizeof(char) * strlen(nom));
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
    n->est_dossier = false;
    estAlpha(nom);
    memcpy(n->nom, nom, sizeof(char) * strlen(nom));
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

void print(noeud* courant){
    assert(courant->racine != NULL);
    puts("\nArbre :\n");
    printRacine(courant->racine, "");
}

void printRacine(noeud* courant, char* tab){   // on commence avec la racine
    if (courant == NULL) return;

    int n = nbFils(courant->fils);
    if (n == 0) printf("%sNoeud : '%s' (%s), %d fils", tab, courant->nom, courant->est_dossier ? "D" : "F", n);
    else printf("%sNoeud : '%s' (%s), %d fils :", tab, courant->nom, courant->est_dossier ? "D" : "F", nbFils(courant->fils));

    liste_noeud* l = courant->fils;
    while (l != NULL){
        if (l->suiv == NULL) printf(" %s (%s)", l->noeud->nom, l->noeud->est_dossier ? "D" : "F");
        else printf(" %s (%s),", l->noeud->nom, l->noeud->est_dossier ? "D" : "F");
        l = l->suiv;
    }
    printf("\n");
    l = courant->fils;
    while (l != NULL){
        printRacine(l->noeud, concat(tab, "|    "));
        l = l->suiv;
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

void* cd(noeud* courant, char* chemin){
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
    char* premier_mot = malloc(sizeof(char)*len);
    assert(premier_mot != NULL);
    memmove(premier_mot, chemin, sizeof(char)*len);

    while(liste != NULL){
        if (strcmp(liste->noeud->nom, premier_mot) == 0)
            return cd(liste->noeud, reste);
        liste = liste->suiv;
    }

    return NULL;
}

void rm(noeud* courant, char* chemin){
    rmAux(courant, chemin, courant);
}

void rmAux(noeud* courant, char* chemin, noeud* origin){
    if (chemin[0] == '\0'){
        printf("Erreur, il faut passer un chemin non vide en parametre.\n");
        return;
    }
    if (chemin[0] == '/') {rmAux(courant->racine, chemin+1, origin); return;}
    if (chemin[0] == '.'){
        if (chemin[1] == '.' && chemin[2] == '/') {rmAux(courant->pere, chemin+3, origin); return;}
        if (chemin[1] == '/') {rmAux(courant, chemin+2, origin); return;}
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
            rmAux(liste->noeud, reste, origin);
            return;
        }
        liste = liste->suiv;
    }

    printf("'%s' n'existe pas dans le dossier. Il ne peut pas être supprimé.\n", premier_mot);
    return;
}

void cp(noeud* courant, char* chemin1, char* chemin2) {
    noeud* noeud1 = cd(courant, chemin1);
    if (noeud1 == NULL ) {
        printf("%s n'existe pas \n", chemin1);
        return;
    }
    if (!noeud1->est_dossier){
        printf("%s n'est pas un dossier \n", chemin1);
        return;
    }

    char* nom_nv_noeud = get_nom_fichier(chemin2);
    char* chemin_destination = get_chemin_fichier(chemin2);
    noeud* noeud_destination = cd(courant, chemin_destination);

    if (noeud_destination == NULL || !noeud_destination->est_dossier) {
        printf("Le dossier de destination n'existe pas \n");
        return;
    }
    if (trouve_fils(noeud_destination, nom_nv_noeud) != NULL){
        printf("Le dossier de destination a un fils ayant le même nom que le nouveau noeud\n");
        return;
    }
    if (appartient_sous_arbre(noeud_destination, noeud1->fils)) {
        printf("Le noeud à copier fait partie du sous-arbre du noeud de destination\n");
        return;
    }
    noeud* copie_noeud = copier_noeud(noeud1);
    strcpy(copie_noeud->nom, nom_nv_noeud);
    copie_noeud->pere = noeud_destination;
    ajouter_fils(noeud_destination, copie_noeud);
}

void mv(noeud *courant, char* chemin1, char* chemin2){
    cp(courant, chemin1, chemin2);
    rm(courant, chemin1);
}

