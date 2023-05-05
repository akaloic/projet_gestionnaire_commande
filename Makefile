CC = gcc
CFLAGS = -Wall
DEPS = auxiliare.h commande.h noeud.h
EXEC = arbre

all : $(EXEC)

auxiliare.o : auxiliare.c $(DEPS)
	$(CC) -c -o $@ $< $(CFLAGS)

commande.o : commande.c $(DEPS)
	$(CC) -c -o $@ $< $(CFLAGS)

noeud.o : noeud.c $(DEPS)
	$(CC) -c -o $@ $< $(CFLAGS)

arbre.o : arbre.c $(DEPS)
	$(CC) -c -o $@ $< $(CFLAGS)

$(EXEC) : arbre.o auxiliare.o commande.o noeud.o
	$(CC) -o $@ $^ $(CFLAGS)

clean :
	rm -rf $(EXEC) *.o