#ifndef _DECOMP_H_
#define _DECOMP_H_

/* pseudo-constantes pour définir le prochain type de donnée à lire dans l'entête du fichier compressé */
#define CHAR 0
#define TAILLE 1
#define CODE 2

/* permet d'avancer dans l'arbre de huffman avec un pointeur parcours sur un noeud */
/* cette fonction écrit aussi dans le fichier? ou retourne le char lu? */
/* ATENTSION */
noeud * lire_bit_arbre(noeud * arbre_huffman, noeud * parcours, int bit);

/* insère une feuille c dans l'arbre de Huffman grâce à son codage */
void inserer_arbre(noeud * a, char c, char * code, int pos);

/* crée et retourne un arbre de Huffman à partir d'un alphabet */
noeud * creer_huffman_inverse(noeud * alphabet[256]);

/* lit l'entête du fichier en paramètre, reconstruit l'arbre de huffman (et le renvoie) et écrit dans un fichier */
noeud * creer_decompresse(FILE * f);

#endif /* _DECOMP_H_ */