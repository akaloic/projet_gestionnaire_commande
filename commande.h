#ifndef COMMANDE_H
#define COMMANDE_H

extern void mkdir(noeud *, char *);
extern void ls(noeud *);
extern void touch(noeud *, char *);
extern void rm(noeud *, char *, noeud *);
extern void pwd(noeud *);
extern void print(noeud *, char []);
extern noeud* cd(noeud *, char *);
extern void cp(noeud *, char *, char *);
extern void mv(noeud *, char *, char *);

#endif