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

/* NOUVEAU, pour v3 */
/* v3: soit FICHIER soit FICHIERS soit DOSSIER */
/* dans v3: erreur si il y a un sous-dossier à l'intérieur de dossier? */
void lister_contenu_dossier(char * argv[]) {
    DIR * d;
    struct dirent *dir;
    char dir_path[100];
    int i = 1;
        
    d = opendir(argv[3]);
    if (d) {
        
        while ((dir = readdir(d)) != NULL) {
            /* exclure . et .. */
            if (strcmp(dir->d_name, ".") != 0 && strcmp(dir->d_name, "..") != 0) {

                /* recréer le chemin d'accès du fichier */
                strcpy(dir_path, argv[3]);
                strcat(dir_path, "/"); /* check si le / y est déjà? OSEF CA MARCHE AVEC //// */
                strcat(dir_path, dir->d_name);
                
                if (!est_fichier(dir_path)) {
                    printf("erreur: %s est un dossier!\n", dir->d_name);
                    /* return; */
                }
                printf("fichier %d: %s\n", i, dir->d_name);
                i++;
            }
        }
        closedir(d);
        
    }
}

/* NOUVEAU, pour v4 */
/* (récursive) */
void lister_contenu_dossier_sous_dossiers(char * chemin, int indente) {
    DIR * d;
    struct dirent *dir;
    char dir_path[100];
    int i = 1;

    int j;
        
    d = opendir(chemin);
    if (d) {
        
        while ((dir = readdir(d)) != NULL) {
            /* exclure . et .. */
            if (strcmp(dir->d_name, ".") != 0 && strcmp(dir->d_name, "..") != 0) {

                /* recréer le chemin d'accès du fichier */
                strcpy(dir_path, chemin);
                strcat(dir_path, "/"); /* check si le / y est déjà? OSEF CA MARCHE AVEC //// */
                strcat(dir_path, dir->d_name);

                for (j = 0; j < indente; j++) printf("\t");
                if (!est_fichier(dir_path)) {
                    printf("%s est un dossier:\n", dir->d_name);
                    lister_contenu_dossier_sous_dossiers(dir_path, indente+1);
                    /* return; */
                }
                printf("fichier %d: %s\n", i, dir->d_name);
                i++;
            }
        }
        closedir(d);
        
    }
}

/* NOUVEAU, pour v5 */
void lister_fichiers_arborescences(char * argv[], int argc) {
    int i;

    /* test ouverture fopen du fichier avant? */
    for (i = 3; i < argc; i++) {
        /* possible de vérifier les erreurs ici si le fichier n'existe pas */
        printf("ARGUMENT %d : est_fichier = %d\n", i-2, est_fichier(argv[i]));
        
        if (est_fichier(argv[i])) {
            /* fichier */
            printf("fichier %d : %s\n", i-2, argv[i]);
        }
        else {
            /* dossier */
            lister_contenu_dossier_sous_dossiers(argv[i], 0);
        }
    }
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

        /* lister_fichiers(argv, argc); */

        /* lister_contenu_dossier(argv); */

        /* lister_contenu_dossier_sous_dossiers(argv[3], 0); */

        lister_fichiers_arborescences(argv, argc);

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
    
    exit(EXIT_SUCCESS);
}