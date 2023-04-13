#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>
#include <assert.h>
#include <ctype.h>
#include "auxiliare.h"

void estAlpha(char *nom){
    for (int i = 0; i < strlen(nom); i++){
        if (isalnum(nom[i]) == 0){
            printf("Le caractere '%c' n'est pas estAlphanumérique.", nom[i]);
            return;            
        }
    }
}

char *concat(char *s1, char *s2) {
    char *result = (char *)malloc(strlen(s1) + strlen(s2) + 1);
    strcpy(result, s1);
    strcat(result, s2);
    return result;
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