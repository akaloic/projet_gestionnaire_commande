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

char concat(char *s1, char *s2) {
    char *result = (char *)malloc(strlen(s1) + strlen(s2) + 1);
    strcpy(result, s1);
    strcat(result, s2);
    result[strlen(s1) + strlen(s2)] = '\0';
    return result;
}

//cette fonction donne le nom a la fin du chemin 
char* getLastName(char* chemin) {
    char* nom = strrchr(chemin, '/');
    nom = (nom == NULL) ? chemin : nom+1;
    return nom;
}

//enleve le dernier nom
char* withoutLastName(char* chemin) {
    char *x = strrchr(chemin, '/');
    if (x == NULL) return chemin;

    char *sansDernier = malloc(sizeof(char)*(strlen(chemin)+1));
    assert(sansDernier != NULL);
    sansDernier[strlen(chemin)-strlen(x)] = '\0';
    strncpy(sansDernier, chemin, strlen(chemin)-strlen(x));

    return sansDernier;
}