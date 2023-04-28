READ ME : 

il faudra un fichier readme.txt expliquant comment compiler
 et exécuter votre projet et le cas échéant détaillant les 
 éventuels problèmes dont souffre votre projet.

gcc -Wall -c commande.c noeud.c auxiliare.c arbre.c   
gcc -Wall -o arbre arbre.o commande.o noeud.o auxiliare.o
./arbre texte.txt