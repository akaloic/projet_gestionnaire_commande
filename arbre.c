#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>
#include <assert.h>
#include <ctype.h>

#define MAX_NOM_TAILLE 100

struct noeud;
struct liste_noeud;

struct noeud{
    bool est_dossier;
    char nom[MAX_NOM_TAILLE];
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
        puts("allocation null");
        assert(n != NULL);
    }
}

void verifListe(liste_noeud *n){
    if (n == NULL){
        puts("allocation null");
        assert(n != NULL);
    }
}

void estAlpha(char *nom){
    for (int i = 0; i < strlen(nom); i++){
        if (isalnum(nom[i]) == 0){
            printf("Le caractere '%c' n'est pas estAlphanumérique.", nom[i]);
            return;            
        }
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

noeud* cd(noeud *courant, char *chemin) {
    if (!courant->est_dossier || courant->fils == NULL) return NULL;
    if (chemin[0] == '/'){
        courant = courant->racine;
        if (strlen(chemin) < 2) return courant;     // "cd /"
        chemin++;
    }
    char* next = strchr(chemin, '/');
    liste_noeud* liste = courant->fils;
    if (next == NULL){
        while (liste != NULL){
            if (strcmp(liste->noeud->nom, chemin) == 0) return liste->noeud;    // "cd exemple"
            liste = liste->suiv;
        }
        return NULL;
    }
    int len = strlen(chemin) - strlen(next); 
    char* premier_mot = malloc(sizeof(char)*len);
    memmove(premier_mot, chemin, sizeof(char)*len);
    while(liste != NULL){
        if (strcmp(liste->noeud->nom, premier_mot) == 0)
            return cd(liste->noeud, next);
        liste = liste->suiv;
    }
    return NULL;
}



//int main(int nbr, char *args) //fichier texte + scanf("..")
int main(){
    noeud *racine = malloc(sizeof(noeud));
    verifNoeud(racine);
    racine->est_dossier = true;
    memcpy(racine->nom, "", sizeof(char) * strlen(""));
    racine->pere = NULL;
    racine->racine = racine;
    racine->fils = NULL;

    char *cours = "Cours";
    char *td = "Td";
    char *edt = "Edt";
    char *projetC = "ProjetC";
    char *anglais = "Anglais";
    char *td1 = "TD1";
    char *td2 = "TD2";

    mkdir(racine, cours);
    mkdir(racine, td);
    mkdir(racine, edt);
    //ls(racine);

    racine = cd(racine, cours);
    mkdir(racine, projetC);
    mkdir(racine, anglais);
    ls(racine);

    return EXIT_SUCCESS;    
}