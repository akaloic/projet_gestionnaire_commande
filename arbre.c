#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <string.h>
#include <assert.h>

#include "noeud.h"
#include "auxiliare.h"
#include "commande.h"

#define MAX_PATH 100

int main(int argc, char *argv[]){
    noeud *courant = malloc(sizeof(noeud));
    assert(courant != NULL);
    courant->est_dossier = true;
    memcpy(courant->nom, "", sizeof(char) * strlen(""));
    courant->pere = courant;
    courant->racine = courant;
    courant->fils = NULL;

    FILE *file = fopen(argv[1], "r"); // ouvre le fichier donné en parametre et le lit
    if (file == NULL){
        perror("Probleme ouverture fichier");
        return EXIT_FAILURE;
    }

    char *line = malloc(sizeof(char) * MAX_PATH);
    assert(line != NULL);
    char *commande = malloc(sizeof(char) * MAX_PATH);
    assert(commande != NULL);
    char *arg1 = malloc(sizeof(char) * MAX_PATH);
    assert(arg1 != NULL);
    char *arg2 = malloc(sizeof(char) * MAX_PATH);
    assert(arg2 != NULL);

    while (fgets(line, MAX_PATH, file) != NULL){

        *commande = '\0';
        *arg1 = '\0';
        *arg2 = '\0';
        sscanf(line, "%s %s %s", commande, arg1, arg2);

        if (strcmp(commande, "mkdir") == 0){
            mkdir(courant, arg1);
        }
        else if (strcmp(commande, "touch") == 0){
            touch(courant, arg1);
        }
        else if (strcmp(commande, "cd") == 0){
            courant = cd(courant, arg1);
        }
        else if (strcmp(commande, "pwd") == 0){
            pwd(courant);
        }
        else if (strcmp(commande, "rm") == 0){
            rm(courant, arg1);
        }
        else if (strcmp(commande, "print") == 0){
            print(courant);
        }
        else if (strcmp(commande, "mv") == 0){
            mv(courant, arg1, arg2);
        }
        else if (strcmp(commande, "cp") == 0){
            cp(courant, arg1, arg2);
        }
        else if(strcmp(commande, "ls")){
            ls(courant);
        }
        else{
            printf("%s : commande kinconnue\n", commande);
        }
    }

    if (fclose(file) != 0){
        perror("Probleme fermeture fichier");
        return EXIT_FAILURE;
    }

    free(courant);
    free(commande);
    free(line);
    free(arg1);
    free(arg2);
    
    
    /*
    // CELA CREER ARBRE DE FIGURE 1
    mkdir(courant, "Cours");
    courant = cd(courant, "Cours");
    mkdir(courant, "ProjetC");
    mkdir(courant, "Anglais");
    courant = cd(courant, "");
    touch(courant, "Edt");
    cp(courant, "Cours", "/Td");
    pwd(courant);
    rm(courant, "/Td/ProjetC");
    rm(courant, "/Td/Anglais");
    courant = cd(courant, "Td");
    mkdir(courant, "td1");
    mkdir(courant, "td2");
    print(courant);
    // CELA CREER ARBRE DE FIGURE 1
    */

    return EXIT_SUCCESS;    
}