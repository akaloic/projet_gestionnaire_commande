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

    if (chemin[0] == '/') return cd(courant->racine, chemin+1);
    if (chemin[0] == '.'){
        if (chemin[1] == '.') return cd(courant->pere, chemin+2);
        if (chemin[1] == '/') return cd(courant, chemin+2);
        else return cd(courant, chemin+1);
    }

    char* next = strchr(chemin, '/');
    liste_noeud* liste = courant->fils;

    if (next == NULL){
        while (liste != NULL){
            if (strcmp(liste->noeud->nom, chemin) == 0){
                if (liste->noeud->est_dossier) return liste->noeud;
                else {
                    printf("'%s' n'est pas un dossier. Le dossier courant reste à la même place.\n", liste->noeud->nom);
                    return courant;
                }
            }
            liste = liste->suiv;
        }
        return courant;
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

char *concat(char *s1, char *s2) {
    char *result = (char *)malloc(strlen(s1) + strlen(s2) + 1);
    strcpy(result, s1);
    strcat(result, s2);
    return result;
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

void print(noeud* courant){
    printRacine(courant->racine, "");
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

//cette fonction donne le nom a la fin du chemin 
char* get_nom_fichier(char* chemin) {
    char* nom = strrchr(chemin, '/');
    if (nom == NULL) {
        nom = chemin;
    } else {
        nom++;
    }
    return nom;
}

//enleve le dernier nom
char* get_chemin_fichier(char* chemin) {
    char* chemin_fichier = strrchr(chemin, '/');
    if (chemin_fichier == NULL) {
        chemin_fichier = chemin;
    } else {
        chemin_fichier++;
        char* chemin2 = malloc(strlen(chemin) + 1);
        strncpy(chemin2, chemin, chemin_fichier - chemin - 1);
        chemin2[chemin_fichier - chemin - 1] = '\0';
        chemin_fichier = chemin2;
    }
    return chemin_fichier;
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

// verifie si n appartien au sous arbre
bool appartient_sous_arbre(noeud* n, liste_noeud* arbre) {
    if (arbre == NULL) {
        return false;
    }
    while (arbre != NULL) {
        if (n == arbre->noeud) {
            return true;
        }
        if (appartient_sous_arbre(n,arbre->noeud->fils)) {
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



void cp(noeud* courant, char* chemin1, char* chemin2) {
    noeud* noeud1 = cd(courant, chemin1);
    if (noeud1 == NULL ) {
        printf("%s n'existe pas \n", chemin1);
        return;
    }
    if (noeud1->est_dossier == false){
        printf("%s n'est pas un dossier \n", chemin1);
        return;
    }
    char* nom_nv_noeud = get_nom_fichier(chemin2);
    char* chemin_destination = get_chemin_fichier(chemin2);
    noeud* noeud_destination = cd(courant, chemin_destination);
    if (noeud_destination == NULL || noeud_destination->est_dossier == false ) {
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
    ajouter_fils(noeud_destination, copie_noeud);
}



//int main(int nbr, char *args) //fichier texte + scanf("..")
int main(){
    noeud *racine = malloc(sizeof(noeud));
    assert(racine != NULL);
    racine->est_dossier = true;
    memcpy(racine->nom, "", sizeof(char) * strlen(""));
    racine->pere = racine;
    racine->racine = racine;
    racine->fils = NULL;
    mkdir(racine, "Cours");
    mkdir(racine, "Td");
    mkdir(racine, "Edt");
    racine = cd(racine, "Cours");
    mkdir(racine, "ProjetC");
    mkdir(racine, "Anglais");
    racine = cd(racine, "../Td");
    touch(racine, "Td1");
    touch(racine, "Td2");
    racine = cd(racine, "/.");
    print(racine);
}