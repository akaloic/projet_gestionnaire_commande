#ifndef NOEUD_H
#define NOEUD_H

struct noeud{
    bool est_dossier;
    char nom[100];
    struct noeud *pere; 
    struct noeud *racine;
    struct liste_noeud *fils;
};
typedef struct noeud noeud;

struct liste_noeud{
    struct noeud *noeud;
    struct liste_noeud *suiv;
};
typedef struct liste_noeud liste_noeud;

extern void freeFils(noeud *);
extern noeud* trouve_fils(noeud*, char*);
extern int nbFils(liste_noeud *);
extern bool appartient_sous_arbre(noeud* noeud, liste_noeud* arbre);
extern void ajouter_fils(noeud *pere, noeud *fils);
extern noeud* copier_noeud(noeud *n);
extern void ajout_noeud_a_liste(noeud *, liste_noeud **);
extern bool estParent(noeud *, noeud *);
extern void freeListeNoeud(liste_noeud *);

#endif