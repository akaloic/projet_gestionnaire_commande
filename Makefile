CC = gcc
CFLAGS = -Wall
DEPS = auxiliare.h commande.h noeud.h
EXEC = arbre

all : $(EXEC)

auxiliare.o : auxiliare.c auxiliare.h
	$(CC) $(CFLAGS) -c auxiliare.c

commande.o : commande.c commande.h
	$(CC) $(CFLAGS) -c commande.c

noeud.o : noeud.c noeud.h
	$(CC) $(CFLAGS) -c noeud.c

arbre.o : arbre.c $(DEPS)
	$(CC) $(CFLAGS) -c arbre.c

arbre : arbre.o auxiliare.o commande.o noeud.o
	$(CC) $(CFLAGS) -o arbre arbre.o auxiliare.o commande.o noeud.o

clean :
	rm -rf $(EXEC) *.o