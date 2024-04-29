#include <stdio.h>
#include <stdlib.h>
#include <limits.h>
#include <MLV/MLV_all.h>
#include <stddef.h>

#include "headers/arbre.h"
#include "headers/comp.h"
#include "headers/decomp.h"
#include "headers/utils.h" /* pour taille */


/* Affiche la documentation du logiciel. */
void afficher_doc(){
    printf("Nom :\n\t./huffman : compression et décompression de fichier utilisant le codage de huffman\n\n");
    printf("Utilisation :\n\t./huffman [OPTION] [FICHIER]...\n\n");
    printf("Description :\n\tAffichage de cette documentation :\n\t\t./huffman -h\n\n");
    printf("\tCompression de fichier(s) ou dossier(s) :\n\t\t./huffman -c nom_archive liste_fichiers_ou_dossiers_a_compresser\n\n");
    printf("\tDécompression d'une archive .comphuff :\n\t\t./huffman -d archive.comphuff [dossier_cible]\n\n");
}


/* Affiche un message d'erreur en cas de problème d'argument */
void usage(char * s){
    printf("Usage %s : pour plus de détails, utilisez l'option -h\n", s);
}


int main(int argc, char * argv[]) {

    FILE * fic = NULL;
    char * chemin_dossier = NULL;
    
    
    if ( (argc < 2) || (taille(argv[1]) != 2) || (argv[1][0] != '-') ) {
        usage(argv[0]);
        exit(EXIT_FAILURE);
    }

    switch (argv[1][1]){
    case 'c':
        if (argc < 3){
            fprintf(stderr, "Veuillez préciser un nom d'archive et un fichier à compresser\n");
            exit(EXIT_FAILURE);
        }

        if (argc < 4) {
            fprintf(stderr, "Veuillez préciser un fichier à compresser\n");
            exit(EXIT_FAILURE);
        }

        /* compression */

        /* ouverture du fichier à compresser */
        fic = fopen(argv[3], "r");
        if (fic == NULL) {
            fprintf(stderr, "Erreur main: erreur lors de l'ouverture du fichier \"%s\"\n", argv[3]);
            exit(EXIT_FAILURE);
        }

        boucle_compresse(fic, argv[2]); /* argv[2] = nom du fichier */
    
        /* on ferme le fichier */
        fclose(fic);
    
        /* afficher_arbre_graphique(arbre_huffman[0]); */
        
        break;
    case 'd':
        if (argc < 3) {
            fprintf(stderr, "Veuillez préciser une archive .comphuff à décompresser\n");
            exit(EXIT_FAILURE);
        }

        /* ouverture du fichier à décompresser */
        fic = fopen(argv[2], "r");
        if (fic == NULL) {
            fprintf(stderr, "Erreur main: erreur lors de l'ouverture du fichier \"%s\"\n", argv[2]);
            exit(EXIT_FAILURE);
        }

        /* decomp de argv[2] */
        /* récupération du chemin du répertoire dans lequel décompresser (si il existe) */
        if (argc >= 4) {
            chemin_dossier = argv[3];
        }
        boucle_decompresse(fic, argv[2], chemin_dossier);

        /* on ferme le fichier */
        fclose(fic);
        
        
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