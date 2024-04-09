#include <stdio.h>
#include <stdlib.h>
#include <limits.h>
#include <MLV/MLV_all.h>
#include <stddef.h>

#include "headers/arbre.h"
#include "headers/comp.h"
#include "headers/decomp.h"


/* Affiche la documentation du logiciel. */
void afficher_doc(){
    printf("Nom :\n\t./huffman : compression et décompression de fichier utilisant le codage de huffman\n\n");
    printf("Utilisation :\n\t./huffman [OPTION] [FICHIER]...\n\n");
    printf("Description :\n\tAffichage de cette documentation :\n\t\t./huffman -h\n\n");
    printf("\tCompression de fichier(s) ou dossier(s) :\n\t\t./huffman -c nom_archive liste_fichiers_ou_dossiers_a_compresser\n\n");
    printf("\tDécompression d'une archive .comphuff :\n\t\t./huffman -d archive.comphuff [dossier_cible]\n\n");
}


/* Calcule la taille de s,
Et la retourne. */
int taille(char *s){
    char *chaine = s;
    int t = 0;

    while (chaine[0] != '\0'){
        t++;
        chaine++;
    }

    return t;
}


/* Affiche un message d'erreur en cas de problème d'argument */
void usage(char * s){
    printf("Usage %s : pour plus de détails, utilisez l'option -h\n", s);
}


int main(int argc, char * argv[]) {

    FILE * fic = NULL;
    int tab[256]; /* nombre d'occurences de chaque caractère */
    noeud * arbre_huffman[256]; /* pointeurs vers des noeuds */
    int n_huffman; /* taille du tableau arbre_huffman = nombre de feuilles à l'origine = nombre de caractères */

    int n;
    int i;
    
    noeud * alphabet[256];
    
    
    if ( (argc < 2) || (taille(argv[1]) != 2) || (argv[1][0] != '-') ) {
        usage(argv[0]);
        exit(EXIT_FAILURE);
    }

    switch (argv[1][1]){
    case 'c':
        if (argc < 3){
            fprintf(stderr, "Veuillez préciser un nom d'archive et une liste de fichiers ou dossiers à compresser\n");
            exit(EXIT_FAILURE);
        }
        
        if (argc < 4) {
            fprintf(stderr, "Veuillez préciser une liste de fichiers ou dossiers à compresser\n");
            exit(EXIT_FAILURE);
        }

        /* compression */
        fic = fopen(argv[2], "r");
        if (fic == NULL) {
            fprintf(stderr, "Erreur main: erreur lors de l'ouverture du fichier \"%s\"\n", argv[1]);
            exit(EXIT_FAILURE);
        }

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
        
        break;
    case 'd':
        if (argc < 3) {
            fprintf(stderr, "Veuillez préciser une archive .comphuff à décompresser\n");
            exit(EXIT_FAILURE);
        }

        /* decomp de argv[2] */
        
        break;
    case 'h':
        afficher_doc();
        break;
    default:
        fprintf(stderr, "Erreur, option inconnu\n");
        break;
    }
    
    exit(EXIT_SUCCESS);
}