#ifndef _DECOMP_H_
#define _DECOMP_H_

/* pseudo-constantes pour définir le prochain type de donnée à lire dans l'entête du fichier compressé */
#define CHAR 0
#define TAILLE 1
#define CODE 2


/* Permet d'avancer dans l'arbre de huffman avec un pointeur parcours sur un noeud.
   S'occupe également d'écrire dans le fichier décompressé.
Retourne le noeud de parcours. */
noeud * lire_bit_arbre(noeud * arbre_huffman, noeud * parcours, int bit, FILE * fic);

/* Insère une feuille c dans l'arbre de Huffman grâce à son codage. */
void inserer_arbre(noeud * a, int c, char * code, int pos);

/* Crée l'arbre de Huffman à partir d'un alphabet,
et le retourne. */
noeud * creer_huffman_inverse(noeud * alphabet[256], int debut);

/* Permet de décompresser le fichier binaire f formaté : dépacement, nombre de caractères différents, structure alphabet, contenu compressé.
Retourne l'abre de codage associé au fichier f compressé. */
noeud * boucle_decompresse(FILE * f, char * nom_decomp, char * chemin_dossier);

#endif /* _DECOMP_H_ */
