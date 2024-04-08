#ifndef _ARBRE_H_
#define _ARBRE_H_

#define WIDTH 1000
#define HEIGHT 800
#define AFF_DEC_Y 50
#define AFF_DEC_X 80

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

noeud * creer_arbre(char c, noeud * filsgauche, noeud * filsdroit);

int est_feuille(noeud * n);

void afficher_arbre_aux(noeud * a, int profondeur, int droit);

void afficher_arbre(noeud * a);

void affiche_car(char c);

void afficher_arbre_graphique(noeud * a);

void afficher_arbre_graphique_aux(noeud * a, int pos_x, int pos_y, int prev_x, int prev_y, int dec_x);

#endif /* _ARBRE_H_ */