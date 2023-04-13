#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <string.h>
#include <assert.h>
#include "noeud.h"
#include "auxiliare.h"
#include "commande.h"

//int main(int nbr, char *args) //fichier texte + scanf("..")
int main(){
    /*
    FILE *flux = fopen(args[1], "r"); // ouvre le fichier donné en parametre et le lit
    if (flux == NULL){
        perror("Probleme ouverture fichier");
    }else{
        
    }*/

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

    return EXIT_SUCCESS;    
}