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
    int codage; /* nouveau codage du caractère */
    int nb_bits; /* nombre de bits sur lequel est codé le caractère */
    
    struct noeud * gauche; /* lit un 0 */
    struct noeud * droit; /* lit un 1 */
} noeud;


/* Alloue la mémoire pour une feuille.
Retourne le nouveau noeud créé. */
noeud * creer_feuille(int *tab, int index);

/* Crée un nouvelle arbre, sont fils gauche sera filsgauche, sont fils droit sera filsdroit.
Retourne le nouveau noeud créé. */
noeud * creer_arbre(char c, noeud * filsgauche, noeud * filsdroit);

/* Revoie 1 si n est une feuille, 0 sinon. */
int est_feuille(noeud * n);

/* Fonction récursive pour afficher l'arbre a sur un terminal. */
static void afficher_arbre_aux(noeud * a, int profondeur, int droit);

/* Affiche l'abre a sur un terminal. */
void afficher_arbre(noeud * a);

/* Affiche les principaux caractères invisibles sous une forme visible. */
void affiche_car(char c);

/* Affiche l'abre a dans une fenêtre MLV. */
void afficher_arbre_graphique(noeud * a);

/* Fonction récursive pour afficher l'arbre a dans une fenêtre MLV. */
static void afficher_arbre_graphique_aux(noeud * a, int pos_x, int pos_y, int prev_x, int prev_y, int dec_x);

#endif /* _ARBRE_H_ */