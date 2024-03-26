#include <stdio.h>
#include <stdlib.h>

#include "arbre.h"


void affichage_code(int nbr_bits, int codage){
    if (nbr_bits <= 0){
        return;
    }

    affichage_code(nbr_bits - 1, codage / 2);
    /* affichage après l'appel récursif pour l'afficher à l'endroit */
    printf("%d", codage % 2);
}


void creer_code(noeud * element, int code, int profondeur, noeud * alphabet[256]){
    if (est_feuille(element)){
        element -> codage = code;
        element -> nb_bits = profondeur;
        
        affichage_code(profondeur, code);
        /* stockage dans la structure alphabet */
        alphabet[element -> c] = element;
    }
    else {
        creer_code(element -> gauche, code * 2, profondeur + 1);
        creer_code(element -> droit, code * 2 + 1, profondeur + 1);
    }
}

int main(){

    affichage_code(1, 0);
    printf("\n");
    affichage_code(2, 0);
    printf("\n");
    affichage_code(4, 0);
    printf("\n");
    affichage_code(5, 5);
    printf("\n");
    affichage_code(8, 69);
    printf("\n");

    exit(EXIT_FAILURE);
}