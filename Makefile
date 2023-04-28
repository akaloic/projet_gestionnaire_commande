CC=gcc 
WAL=-Wall
DEPS=aux.h commandes.h noeud.h
OBJ=aux.o commandes.o noeud.o
ARB=arbre

all : $(ABR)

%.o : %.c $(DEPS)
	$(CC) $(WAL) -c -o $@ $<

$(ABR) : $(ABR).o auxiliare.o commande.o noeud.o
	$(CC) $(WAL) -o $@ $^

clean :
	rm -rf $(ABR) *.o


