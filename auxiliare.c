#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>
#include <assert.h>
#include <ctype.h>
#include "auxiliare.h"

/**
 * @brief Vérifie si le nom est alphanumérique
 * 
 * ex : "abc" -> true
 *     "a.txt" -> false  (car contient un point)
 * 
 * @param nom   nom à vérifier
 */
void estAlpha(char *nom){
    for (int i = 0; i < strlen(nom); i++){
        if (isalnum(nom[i]) == 0){
            printf("Le caractere '%c' n'est pas alphanumérique.", nom[i]);
            return;            
        }
    }
}

/**
 * @brief concatene deux chaines de caractères
 * 
 * ex : s1 = "abc" et s2 = "def" -> "abcdef"
 * 
 * @param s1    premiere chaine de caractères
 * @param s2    deuxieme chaine de caractères
 * @return char*    chaine de caractères concaténée
 */
char *concat(char *s1, char *s2) {
    char *result = (char *)malloc(strlen(s1) + strlen(s2) + 1);
    strcpy(result, s1);
    strcat(result, s2);
    result[strlen(s1) + strlen(s2)] = '\0';
    return result;
}

/**
 * @brief retourne le dernier nom du chemin
 * 
 * ex : /home/user/Documents -> Documents
 * 
 * @param chemin    chemin dont on veut le dernier nom
 * @return char*    dernier nom du chemin
 */
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

/**
 * @brief retourn le chemin sans le dernier nom
 * 
 * ex : /home/user/Documents -> /home/user/
 * 
 * @param chemin    chemin dont on veut enlever le dernier nom
 * @return char*    chemin sans le dernier nom
 */
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
