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

void estAlpha(char *nom){
    for (int i = 0; i < strlen(nom); i++){
        if (isalnum(nom[i]) == 0){
            printf("Le caractere '%c' n'est pas estAlphanumérique.", nom[i]);
            return;            
        }
    }
}

noeud* creerDossier(){
    noeud *n = malloc(sizeof(noeud));
    assert(n != NULL);
    n->est_dossier = true;
    n->pere = NULL;
    n->racine = n;
    n->fils = NULL;
    return n;
}

void mkdir(noeud *courant, char* nom){
    if (courant->est_dossier == false) return;
    noeud *n = malloc(sizeof(noeud));
    assert(n != NULL);
    n->est_dossier = true;
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

noeud* cd(noeud *courant, char *chemin) {
    if (!courant->est_dossier || courant->fils == NULL) return NULL;

    if (chemin[0] == '/' || chemin[0] == '.'){
        int point = 0;
        for (int i = 0; chemin[i] != '\0' && i < 2; i++){
            if (chemin[i] == '/') break;
            if (chemin[i] == '.') point++;
        }
        switch (point){
            case 0 : courant = courant->racine; break;
            case 1 : if (chemin[0] == '/') courant = courant->racine; break;
            case 2 : 
                if (strlen(chemin) > 2){
                    if (chemin[2] == '/') return cd(courant->pere, chemin+3);
                    else return NULL;
                }
                else return courant->pere;
            default: return NULL;   // ce cas n'arrive jamais
        }
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
    assert(premier_mot != NULL);
    memmove(premier_mot, chemin, sizeof(char)*len);

    while(liste != NULL){
        if (strcmp(liste->noeud->nom, premier_mot) == 0)
            return cd(liste->noeud, next);
        liste = liste->suiv;
    }

    return NULL;
}

void touch(noeud* courant, char* nom){
    if (!courant->est_dossier) return;

    noeud* ficher = malloc(sizeof(noeud));
    assert(ficher != NULL);
    ficher->est_dossier = false;
    memcpy(ficher->nom, nom, sizeof(char) * strlen(nom));
    ficher->pere = courant;
    ficher->racine = courant->racine;
    ficher->fils = NULL;

    liste_noeud* l = malloc(sizeof(liste_noeud));
    assert(l != NULL);
    l->noeud = ficher;
    l->suiv = NULL;

    if (courant->fils == NULL) courant->fils = l;
    else{
        liste_noeud* tmp = courant->fils;
        while (tmp->suiv != NULL) tmp = tmp->suiv;
        tmp->suiv = l;
    }
}

void pwd(noeud* courant){
    noeud* d = courant;
    assert(d != NULL);
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

//int main(int nbr, char *args) //fichier texte + scanf("..")
int main(){
    /*
    FILE *flux = fopen(args[1], "r"); // ouvre le fichier donné en parametre et le lit
    if (flux == NULL){
        perror("Probleme ouverture fichier");
    }else{
        
    }*/

    noeud *racine = malloc(sizeof(noeud));
    assert(racine != NULL);
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
    touch(racine, "test.txt");
    ls(racine);
    pwd(racine);

    mkdir(racine, td1);
    racine = cd(racine, td1);
    pwd(racine);

    return EXIT_SUCCESS;    
}