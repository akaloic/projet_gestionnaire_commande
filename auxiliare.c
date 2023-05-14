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
    result[strlen(s1) + strlen(s2)] = '\0';
    return result;
}

// Cette fonction donne le nom à la fin du chemin 
char* getLastName(char* chemin) {
    char* nom = strrchr(chemin, '/');
    if (nom == NULL){
        return chemin;
    }
    nom = (nom == NULL) ? chemin : nom+1;
    char* newNom = malloc(sizeof(char) * (strlen(nom) + 1));
    assert(newNom != NULL);
    strcpy(newNom, nom);
    if (newNom != NULL){
        newNom[strlen(nom)]= '\0';
        return newNom;
    }else{
        return NULL;
    }
}

// Enlever le dernier nom
char* withoutLastName(char* chemin) {
    char* x = strrchr(chemin, '/');
    if (x == NULL) {
        return NULL;
    }
    char* sansDernier = malloc(sizeof(char) * (strlen(chemin) - strlen(x) + 1));
    assert(sansDernier != NULL);
    strncpy(sansDernier, chemin, strlen(chemin) - strlen(x));
    sansDernier[strlen(chemin) - strlen(x)] = '\0';
    return sansDernier;
}
