#include <stdio.h>
#include <stdlib.h>
/* #include <MLV/MLV_all.h> */

#include "headers/arbre.h"


/* Affiche les principaux caractères invisibles sous une forme visible. */
void affiche_car(char c) {
    if (c == '\n') printf("\\n");
    else if (c == ' ') printf("espace");
    else if (c == '\t') printf("tab");
    else printf("%c", c);
}


/* Alloue la mémoire pour une feuille.
Retourne le nouveau noeud créé. */
noeud * creer_feuille(int *tab, int index) {
    noeud * n;
    
    n = (noeud *) malloc (sizeof(noeud));
    if (n == NULL) {
        fprintf(stderr, "Erreur creer_feuille: erreur d'allocation mémoire\n");
        exit(EXIT_FAILURE);
    }

    n->c = index;
    n->occurence = tab[index];
    n->codage = -1; /* ??????? */
    n->nb_bits = 0; /* pas encore défini */

    n->gauche = NULL;
    n->droit = NULL;

    return n;
}


/* Crée un nouvelle arbre, sont fils gauche sera filsgauche, sont fils droit sera filsdroit.
Retourne le nouveau noeud créé. */
noeud * creer_arbre(char c, noeud * filsgauche, noeud * filsdroit) {
  noeud * a;
  a = (noeud *) malloc (sizeof(noeud));
  if (a == NULL) {
    fprintf(stderr, "Erreur de création de l'arbre\n");
    exit(EXIT_FAILURE);
  }

  a->c = c;
  a->gauche = filsgauche;
  a->droit = filsdroit;

  return a;
}


/* Revoie 1 si n est une feuille, 0 sinon. */
int est_feuille(noeud * n) {
    return (n->gauche == NULL && n->droit == NULL);
}


/* Fonction récursive pour afficher l'arbre a dans une fenêtre MLV. */
void afficher_arbre_graphique_aux(noeud * a, int pos_x, int pos_y, int prev_x, int prev_y, int dec_x) {
  printf("lala\n");
}


/* Affiche l'abre a dans une fenêtre MLV. */
void afficher_arbre_graphique(noeud * a) {
  /* MLV_create_window("Arbre binaire", "keyboard events", WIDTH, HEIGHT);

  MLV_draw_filled_rectangle(0, 0, WIDTH, HEIGHT, MLV_COLOR_BLACK);

  afficher_arbre_graphique_aux(a, WIDTH / 2, 50, WIDTH / 2, 50, AFF_DEC_X);

  MLV_actualise_window();

  while (MLV_get_keyboard_state(MLV_KEYBOARD_ESCAPE) != MLV_PRESSED) {
  }

  MLV_free_window();*/
  printf("affiche arbre\n");
}
