#ifndef _COMP_H_
#define _COMP_H_

/* FONCTIONS D'INITIALISATION */
/* Initialise le nombre d'occurences de chaque caractère à 0. */
void initialiser_occurences(int tab[256]);
/* Initialise tous les pointeurs du tableau à NULL. */
void initialiser_arbre_huffman(noeud * arbre_huffman[256]);

/* Compte le nombre d'occurences de chaque caractère dans le fichier fic et place le résultat dans un tableau. */
void occurence(FILE *fic, int tab[256]);
/* Affiche toute les occurences des caractères contenu dans le fichier. */
void afficher_occurences(noeud * arbre_huffman[256]);

/* 4.2.4 */
/* Crée une structure noeud pour chaque caractère contenu dans le fichier.
Retourne la taille du tableau. */
int creer_noeuds_caracteres(int tab[256], noeud * arbre_huffman[256]);

/* Place dans i1 et i2 les indices des deux noeuds aux occurences les plus petites (i1 <= i2). */
void deux_entiers_petits(noeud * arbre_huffman[256], int * i1, int * i2);

/* ########### */
void creer_noeud(noeud * tab[], int taille);
/* Affiche le codage du caractère (int) en binaire. */
void affichage_code(int nbr_bits, int codage);

/* Stocke dans la structure alphabet le codage des caractères. */
/* Mettre 0 à code. */
void creer_code(noeud * element, int code, int profondeur, noeud * alphabet[256]);

/* Écrit dans le fichier fic, à l'aide du buffer. Renvoi le buffer qui sera probablement non vide. */
/* Start doit être n - 1. */
char ecrire_fich(FILE * fic, int *it, char buffer, int code, int start);

/* Crée et écrit dans le fichier compresser avec l'extension .comphuff
le fichier sera de la forme : dépacement, nombre de caractères différent, structure alphabet, contenu compressé. */
void creer_compresse(char * nom_fichier, FILE* fic, int nb_char, noeud * alphabet[256]);

/* boucle principale pour compresser un fichier et l'ajouter à la fin d'un nouveau fichier compressé */
/* on suppose fic déjà ouvert */
void boucle_compresse(FILE * fic, char * nom_fic);

#endif /* _COMP_H_ */