#include <stdio.h>
#include <stdlib.h>

#include "arbre.h"

noeud * creer_feuille(int *tab, int index) {
    noeud * n;

    n = (noeud *) malloc (sizeof(noeud));
    if (n == NULL) {
        fprintf(stderr, "Erreur creer_feuille: erreur d'allocation mémoire\n");
        exit(EXIT_FAILURE);
    }

    n->c = index;
    n->occurence = tab[index];
    n->codage = tab[index]; /* ??????? */
    n->nb_bits = 0; /* pas encore défini */

    n->gauche = NULL;
    n->droit = NULL;

    return n;
}