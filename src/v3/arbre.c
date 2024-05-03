#include <stdio.h>
#include <stdlib.h>
#include <MLV/MLV_all.h>

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


/* Fonction récursive pour afficher l'arbre a sur un terminal. */
void afficher_arbre_aux(noeud * a, int profondeur, int droit) {
  int i;
  
  if (a != NULL) {
    for (i=0; i < profondeur; i++) {
      if (i == profondeur - 1) {
	if (droit == 1) printf(" └");
	else printf(" ├");
	printf("────");
      }

      else {
	printf("      ");
      }
    }
    printf("(");
    affiche_car(a->c);
    printf("-%d)\n", a->occurence);
    afficher_arbre_aux(a->gauche, profondeur + 1, 0);
    afficher_arbre_aux(a->droit, profondeur + 1, 1);
  }
}


/* Affiche l'abre a sur un terminal. */
void afficher_arbre(noeud * a) {
  afficher_arbre_aux(a, 0, 0);
}


/* Fonction récursive pour afficher l'arbre a dans une fenêtre MLV. */
void afficher_arbre_graphique_aux(noeud * a, int pos_x, int pos_y, int prev_x, int prev_y, int dec_x) {
  char aff[3];
  
  if (a != NULL) {
    if (a->c == '\0') {
      aff[0] = '-'; aff[1] = '\0';
    }
    else {
        if (a->c == '\n') { aff[0] = '\\'; aff[1] = 'n'; aff[2] = '\0'; }
        else if (a->c == ' ') { aff[0] = '_'; aff[1] = '\0'; }
        else { aff[0] = a->c; aff[1] = '\0'; }
    }

    /* couleur de ligne qui dépend de la profondeur */
    MLV_draw_line(pos_x, pos_y, prev_x, prev_y, MLV_rgba(255, pos_y - 50, pos_y / 5, 255));

    if (prev_x < pos_x) {
        /* on vient de la gauche */
        afficher_arbre_graphique_aux(a->gauche, pos_x - dec_x / 2, pos_y + AFF_DEC_Y, pos_x, pos_y, dec_x / 2);
        afficher_arbre_graphique_aux(a->droit, pos_x + dec_x, pos_y + AFF_DEC_Y, pos_x, pos_y, dec_x / 2);
    } else {
        /* on vient de la droite */
        afficher_arbre_graphique_aux(a->gauche, pos_x - dec_x, pos_y + AFF_DEC_Y, pos_x, pos_y, dec_x / 2);
        afficher_arbre_graphique_aux(a->droit, pos_x + dec_x / 2, pos_y + AFF_DEC_Y, pos_x, pos_y, dec_x / 2);
    }

    /* MLV_draw_text(pos_x, pos_y, aff, MLV_COLOR_WHITE); */

    if ((a->occurence / 10) == 0) {
        aff[0] = a->occurence + '0';
        aff[1] = '\0';
    } else {
        aff[0] = (a->occurence / 10) + '0';
        aff[1] = (a->occurence % 10)  + '0';
        aff[2] = '\0';
    }
    
    MLV_draw_text(pos_x, pos_y + 15, aff, MLV_COLOR_GRAY);
  }
}


/* Affiche l'abre a dans une fenêtre MLV. */
void afficher_arbre_graphique(noeud * a) {
  MLV_create_window("Arbre binaire", "keyboard events", WIDTH, HEIGHT);

  MLV_draw_filled_rectangle(0, 0, WIDTH, HEIGHT, MLV_COLOR_BLACK);

  afficher_arbre_graphique_aux(a, WIDTH / 2, 50, WIDTH / 2, 50, AFF_DEC_X);

  MLV_actualise_window();

  while (MLV_get_keyboard_state(MLV_KEYBOARD_ESCAPE) != MLV_PRESSED) {
  }

  MLV_free_window();
}
