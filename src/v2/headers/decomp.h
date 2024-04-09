#ifndef _DECOMP_H_
#define _DECOMP_H_

/* pseudo-constantes pour définir le prochain type de donnée à lire dans l'entête du fichier compressé */
#define CHAR 0
#define TAILLE 1
#define CODE 2


/* Permet d'avancer dans l'arbre de huffman avec un pointeur parcours sur un noeud.
Retourne le noeud de parcours. */
noeud * lire_bit_arbre(noeud * arbre_huffman, noeud * parcours, int bit);

/* Insère une feuille c dans l'arbre de Huffman grâce à son codage. */
void inserer_arbre(noeud * a, char c, char * code, int pos);

/* Crée l'arbre de Huffman à partir d'un alphabet,
et le retourne. */
noeud * creer_huffman_inverse(noeud * alphabet[256]);

/* Permet de décompresser le fichier binaire f formaté : dépacement, nombre de caractères différents, structure alphabet, contenu compressé.
Retourne l'abre de codage associé au fichier f compressé. */
noeud * creer_decompresse(FILE * f);

#endif /* _DECOMP_H_ */