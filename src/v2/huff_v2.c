#include <stdio.h>
#include <stdlib.h>
#include <limits.h>
#include <MLV/MLV_all.h>
#include <stddef.h>

#include <dirent.h> /* pour lister les fichiers d'un dossier */
#include <string.h> /* pour comparer les noms de fichiers */

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



/* NOUVEAU, pour v2 */
void lister_fichiers(char * argv[], int argc) {
    int i;

    /* test ouverture fopen du fichier avant? */
    for (i = 3; i < argc; i++) {
        printf("fichier %d : %s\n", i-2, argv[i]);
        /* possible de vérifier les erreurs ici si le fichier n'existe pas */
        printf("\test fichier? %d\n", est_fichier(argv[i]));
    }
}






/* en supposant les arguments valides */
void compression_multifichiers(char * argv[], int argc) {

    int i_fich;
    int fichier_valide = 0;

    for (i_fich = 3; i_fich < argc; i_fich++) {

        fichier_valide = est_fichier(argv[i_fich]);
        /* si le fichier est un dossier ou que le fichier n'existe pas, on ne le traite pas */
        if (fichier_valide == 1) {

            printf("fichier '%s' bien traité\n", argv[i_fich]);
        }
        else {
            printf("erreur: le fichier '%s' n'a pas pu être traité (c'est soit un dossier, soit il n'existe pas)\n", argv[i_fich]);
        }
        
    }
    
}






void ecrire_fichier_dans_fichier_tmp(FILE * fich_tmp, char * chemin) {

    FILE * fich = NULL;

    int c;

    fich = fopen(chemin, "r");
    if (fich == NULL) {
        exit(EXIT_FAILURE);
    }

    /* POUR LA V2: ON ECRIT LE CHEMIN COMME BASENAME */
    fprintf(fich_tmp, "%s\n", nom_fich_base(chemin));

    /* on écrit le contenu du fichier */
    while ((c = fgetc(fich)) != EOF) {
        fprintf(fich_tmp, "%c", c);
    }

    fprintf(fich_tmp, "%c%c%c", 1, 1, 1); /* séparateur */

    fclose(fich);
}

void compiler_dans_fichier_tmp(char * argv[], int argc) {
    
    int i_fich;
    int fichier_valide = 0;

    FILE * fich_tmp = NULL;
    FILE * fich = NULL;

    int c;

    fich_tmp = fopen("tmp.tmpcomp", "w+");
    if (fich_tmp == NULL) {
        exit(EXIT_FAILURE);
    }

    for (i_fich = 3; i_fich < argc; i_fich++) {

        fichier_valide = est_fichier(argv[i_fich]);
        /* si le fichier est un dossier ou que le fichier n'existe pas, on ne le traite pas */
        if (fichier_valide == 1) {

            ecrire_fichier_dans_fichier_tmp(fich_tmp, argv[i_fich]);

            printf("fichier '%s' bien traité\n", argv[i_fich]);
        }
        else {
            printf("erreur: le fichier '%s' n'a pas pu être traité (c'est soit un dossier, soit il n'existe pas)\n", argv[i_fich]);
        }
        
    }

    fclose(fich_tmp);
    
}

void reconstituer_fichiers(char * chemin) {

    FILE * fich = NULL;
    FILE * fich_part = NULL;

    char * line = NULL;
    size_t len = 0;

    int loop = 1;
    int char_loop = 1;

    int c;

    fich = fopen(chemin, "r");
    if (fich == NULL) {
        printf("erreur lors de l'ouverture du fichier\n");
        exit(EXIT_FAILURE);
    }

    while (loop == 1) {
        /* obtenir le nom */
        if (getline(&line, &len, fich) == -1) {
            printf("erreur lors de la lecture du fichier\n");
            loop = 0;
            break;
        }

        /* créer le fichier avec le bon nom */
        /* vérifier que le fichier n'existe pas déjà? */
        fich_part = fopen(line, "w");
        if (fich_part == NULL) {
            printf("erreur lors de la création du fichier %s\n", line);
            exit(EXIT_FAILURE);
        }
        printf("create ok pour le fichier %s\n", line);

        /* on récupère des caractères jusqu'à avoir trois fois l'ASCII 1 (séparateur) */
        char_loop = 0;
        while (char_loop < 3 && (c = fgetc(fich))) {
            if (c == 1) {
                char_loop++;
            } else {
                char_loop = 0;
            }

            if (char_loop < 3) {
                fprintf(fich_part, "%c", c);
            }
        }

        fclose(fich_part);
    }
    while (getline(&line, &len, fich) != -1) {
        printf("%s", line);
    }

    fclose(fich);
    
}







/* Affiche un message d'erreur en cas de problème d'argument */
void usage(char * s){
    printf("Usage %s : pour plus de détails, utilisez l'option -h\n", s);
}


int main(int argc, char * argv[]) {

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

        /* compression_multifichiers(argv, argc); */
        
        compiler_dans_fichier_tmp(argv, argc);

        /* appel de compression */
        printf("compr\n");
        
        break;
    case 'd':
        if (argc < 3) {
            fprintf(stderr, "Veuillez préciser une archive .comphuff à décompresser\n");
            exit(EXIT_FAILURE);
        }
        if (argc < 4) {
            /* décompression sans dossier cible */
            printf("decomp sans dossier cible\n");
        }
        else {
            /* décompression avec dossier cible */
            printf("decomp dossier cible\n");
        }
        
        break;
    case 'h':
        afficher_doc();
        break;
    default:
        fprintf(stderr, "Erreur, option inconnu\n");
        break;
    }


    reconstituer_fichiers("tmp.tmpcomp");
    
    exit(EXIT_SUCCESS);
}