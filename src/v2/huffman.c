#include <stdio.h>
#include <stdlib.h>
#include <limits.h>
#include <string.h>
#include <MLV/MLV_all.h>

#include <stddef.h>

#include "headers/arbre.h"
#include "headers/comp.h"
#include "headers/decomp.h"


void usage(char *s){
    printf("Usage %s : <fichier>\n", s);
}


int main(int argc, char *argv[]) {

    FILE * fic = NULL;
    int tab[256]; /* nombre d'occurences de chaque caractère */
    noeud * arbre_huffman[256]; /* pointeurs vers des noeuds */
    int n_huffman; /* taille du tableau arbre_huffman = nombre de feuilles à l'origine = nombre de caractères */

    int n;
    int i;
    
    noeud * alphabet[256];

    

    if (argc < 2){
        usage(argv[0]);
        exit(EXIT_FAILURE);
    }

    fic = fopen(argv[1], "r");
    if (fic == NULL) {
        fprintf(stderr, "Erreur main: erreur lors de l'ouverture du fichier \"%s\"\n", argv[1]);
        exit(EXIT_FAILURE);
    }



    if (1) { /* SI ON COMPRESSE */

        initialiser_occurences(tab);
        initialiser_arbre_huffman(arbre_huffman);

        /* appel de la fonction occurence */
        occurence(fic, tab);
    
        /* 4.2.5 */
        n_huffman = creer_noeuds_caracteres(tab, arbre_huffman);

        /* on affiche les occurences de chaque caractère (si il y a eu une occurence) */
        afficher_occurences(arbre_huffman);

        /* on crée l'arbre */
        n = n_huffman;
        while (n != 1) {
            creer_noeud(arbre_huffman, 256);
            n--;
            /* debug_huffman(arbre_huffman, n_huffman); */
        }

        /* trouver l'arbre final dans la structure */
        n = 0;
        while (arbre_huffman[n] == NULL) n++;
        /* le premier pointeur (index 0) est l'arbre final */
        arbre_huffman[0] = arbre_huffman[n];

        /* initialisation du tableau de noeud * alphabet */
        for (i = 0; i < 256; i++) {
            alphabet[i] = NULL;
        }

        /* créer l'alphabet */
        creer_code(arbre_huffman[0], 0, 0, alphabet);

        /* créer le fichier compressé */
        /* ATTENTION POUR ETENDRE LES OPTIONS */
        creer_compresse(argv[1], fic, n_huffman, alphabet);
    
        /* on ferme le fichier */
        fclose(fic);
    
        afficher_arbre_graphique(arbre_huffman[0]);

    }
    else {
        /* SI ON DECOMPRESSE */
    }

    exit(EXIT_SUCCESS);

}
