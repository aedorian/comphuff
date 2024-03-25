#ifndef _ARBRE_H_
#define _ARBRE_H_

/* début 4.2 */

typedef struct noeud {
    char c; /* caractère initial */
    int occurence; /* occurence dans le fichier */
    int codage; /* ??? */
    int nb_bits; /* nombre de bits sur lequel est codé le caractère */
    
    struct noeud * gauche; /* lit un 0 */
    struct noeud * droit; /* lit un 1 */
} noeud;

noeud * creer_feuille(int *tab, int index);

#endif /* _ARBRE_H_ */