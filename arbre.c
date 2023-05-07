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
    strcpy(courant->nom, "");
    courant->pere = courant;
    courant->racine = courant;
    courant->fils = NULL;

    FILE *file = fopen(argv[1], "r"); // ouvre le fichier donné en parametre et le lit
    if (file == NULL){
        perror("Probleme ouverture fichier");
        fprintf(stderr, "Erreur lors de l'ouverture du fichier.\n");
        return EXIT_FAILURE;
    }

    char *line = malloc(sizeof(char) * 10000);
    assert(line != NULL);
    char *commande = malloc(sizeof(char) * 10000);
    assert(commande != NULL);
    char *arg1 = malloc(sizeof(char) * 10000);
    assert(arg1 != NULL);
    char *arg2 = malloc(sizeof(char) * 10000);
    assert(arg2 != NULL);

    bool erreur = false;

    while (fgets(line, 10000, file) != NULL){

        *commande = '\0';
        *arg1 = '\0';
        *arg2 = '\0';
        sscanf(line, "%s %s %s", commande, arg1, arg2);

        if (strcmp(commande, "mkdir") == 0){
            if ((*arg2) != '\0') {
                fprintf(stderr, "cd : trop d'arguments\n");
                erreur = true;
                break;
            }else{
                mkdir(courant, arg1);
            }
        }
        else if (strcmp(commande, "touch") == 0){
            if ((*arg2) != '\0') {
                fprintf(stderr, "cd : trop d'arguments\n");
                erreur = true;
                break;
            }else{
                touch(courant, arg1);
            }
        }
        else if (strcmp(commande, "cd") == 0){
            if ((*arg2) != '\0') {
                fprintf(stderr, "cd : trop d'arguments\n");
                erreur = true;
                break;
            }else{
                courant = cd(courant, arg1);
                if (courant == NULL) {
                    fprintf(stderr, "cd : trop d'arguments\n");
                    erreur = true;
                    break;
                }
            }
        }
        else if (strcmp(commande, "pwd") == 0){
            if ((*arg2) != '\0' || (*arg1) != '\0') {
                fprintf(stderr, "cd : trop d'arguments\n");
                erreur = true;
                break;
            }else{
                pwd(courant);
            }
        }
        else if (strcmp(commande, "rm") == 0){
            if ((*arg2) != '\0') {
                fprintf(stderr, "cd : trop d'arguments\n");
                erreur = true;
                break;
            }else{
                rm(courant, arg1, courant);
            }
        }
        else if (strcmp(commande, "print") == 0){
            if ((*arg2) != '\0' || (*arg1) != '\0'){
                fprintf(stderr, "cd : trop d'arguments\n");
                erreur = true;
                break;
            }else{
                puts("ARBRE :\n");
                print(courant, "");
            }
        }
        else if (strcmp(commande, "mv") == 0){
            mv(courant, arg1, arg2);
        }
        else if (strcmp(commande, "cp") == 0){
            cp(courant, arg1, arg2);
        }
        else{
            printf("%s : commande inconnue\n", commande);
        }

        if (erreur) break;
    }

    if (fclose(file) != 0){
        perror("Probleme fermeture fichier");
        fprintf(stderr, "Erreur lors de la fermeture du fichier.\n");
        return EXIT_FAILURE;
    }
    freeFils(courant);
    free(commande);
    free(line);
    free(arg1);
    free(arg2);

    return EXIT_SUCCESS;    
}