#ifndef _COMP_H_
#define _COMP_H_

/* FONCTIONS D'INITIALISATION */
/* initialise le nombre d'occurences de chaque caractère à 0 */
void initialiser_occurences(int tab[256]);
/* initialise tous les pointeurs du tableau à NULL */
void initialiser_arbre_huffman(noeud * arbre_huffman[256]);

/* compte le nombre d'occurences de chaque caractère dans le fichier fic
   et place le résultat dans un tableau */
void occurence(FILE *fic, int tab[256]);

void afficher_occurences(noeud * arbre_huffman[256]);

/* 4.2.4 */
/* crée une structure noeud pour chaque caractère contenu dans le fichier */
int creer_noeuds_caracteres(int tab[256], noeud * arbre_huffman[256]);

/* 4.2.6 */
/* place dans i1 et i2 les indices des deux noeuds aux occurences les plus petites (i1 <= i2) */
void deux_entiers_petits(noeud * arbre_huffman[256], int * i1, int * i2);

/* 4.2.7 */
void creer_noeud(noeud * tab[], int taille);

void affichage_code(int nbr_bits, int codage);

/* mettre 0 à code */
void creer_code(noeud * element, int code, int profondeur, noeud * alphabet[256]);

/* écrit dans le fichier fic, à l'aide du buffer */
/* start doit être n - 1 */
char ecrire_fich(FILE * fic, int *it, char buffer, int code, int start);

/* à modulariser */
void creer_compresse(char * nom_fichier, FILE* fic, int nb_char, noeud * alphabet[256]);

#endif /* _COMP_H_ */