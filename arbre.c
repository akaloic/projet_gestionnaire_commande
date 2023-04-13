#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <string.h>
#include <assert.h>

#include "noeud.h"
#include "auxiliare.h"
#include "commande.h"

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

    char line[50];
    char command[50];
    char path[50];
    int i = 0;

    while(fgets(line, 100, file) != NULL){
        while(line[i] != ' ' && line[i] != '\n') strncat(command, &line[i++], 1);
        i++;
        while(line[i] != '\0' && line[i] != '\n') strncat(path, &line[i++], 1);
        printf("%s %s", command, path);
        /*if (strcmp(command, "mkdir") == 0) mkdir(courant, path);
        else if (strcmp(command, "touch") == 0) touch(courant, path);
        else if (strcmp(command, "cd") == 0) courant = cd(courant, path);
        else if (strcmp(command, "rm") == 0) rm(courant, path);
        else if (strcmp(command, "pwd") == 0) pwd(courant);
        else if (strcmp(command, "ls") == 0) ls(courant);
        else if (strcmp(command, "print") == 0) print(courant);
        else if (strcmp(command, "exit") == 0) break;
        else printf("'%s' : commande inconnue", command);
        */
    }

    /*  // ca marche ça
    fgets(line, 100, file);
    while(line[i] != ' ' && line[i] != '\n') strncat(command, &line[i++], 1);
    i++;
    while(line[i] != '\0' && line[i] != '\n') strncat(path, &line[i++], 1);
    printf("%s", command);
    */

    /*
    noeud *courant = malloc(sizeof(noeud));
    assert(courant != NULL);
    courant->est_dossier = true;
    memcpy(courant->nom, "", sizeof(char) * strlen(""));
    courant->pere = courant;
    courant->racine = courant;
    courant->fils = NULL;

    // CELA CREER ARBRE DE FIGURE 1
    mkdir(courant, "Cours");
    mkdir(courant, "Td");
    touch(courant, "Edt");

    courant = cd(courant, "Cours");
    mkdir(courant, "ProjetC");
    mkdir(courant, "Anglais");

    courant = cd(courant, "../Td");
    touch(courant, "Td1");
    touch(courant, "Td2");

    courant = cd(courant, ""); // ou  'cd(courant, "/");'
    // CELA CREER ARBRE DE FIGURE 1

    courant = cd(courant, "Cours/ProjetC/../Anglais");
    pwd(courant);

    //rm(courant, "Td1"); // Td1 erreur
    rm(courant, "../ProjetC"); // ProjetC OK
    //rm(courant, "../../Cours"); // Cours NON car parent de courant

    print(courant);
    */

    if (fclose(file) != 0){
        perror("Probleme fermeture fichier");
        return EXIT_FAILURE;
    }

    return EXIT_SUCCESS;    
}