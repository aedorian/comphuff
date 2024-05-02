#define _POSIX_C_SOURCE 200809L
#include <stdio.h>
#include <stdlib.h>
#include <limits.h>
#include <MLV/MLV_all.h>
#include <stddef.h>

#include <dirent.h> /* pour lister les fichiers d'un dossier */
#include <string.h> /* pour comparer les noms de fichiers */

#include <sys/stat.h> /* mkdir */

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



/* ----------------------------------------------------------------------- */
/* DEBUT PLUSIEURS FICHIERS */
/* ----------------------------------------------------------------------- */


char * obtenir_nom_dernier_dossier(char * chemin) {

    char * dernier_dossier = (char *) malloc (100 * sizeof(char)); /* dernier bon dossier */
    const char * separators = "/";
    int boucle = 0;

    char * tok = strtok(chemin, separators);
    
    while (tok != NULL) {

        strcpy(dernier_dossier, tok);
        tok = strtok(NULL, separators);
    }
    
    return dernier_dossier;
}

/* pour reconstituer les fichiers: obtient le premier nom de dossier dans la chaîne */
/* exemple: "doss/fich.txt" donnera "doss" */
char * obtenir_nom_dossier(char * chemin) {

    char * dernier_dossier = (char *) malloc (100 * sizeof(char)); /* dernier bon dossier */
    const char * separators = "/";
    int boucle = 0;

    char * tok =  (char *) malloc (100 * sizeof(char));

    tok = strtok(chemin, separators);
    
    while (tok != NULL) {

        strcpy(dernier_dossier, tok);
        tok = strtok(NULL, separators);
    }
    
    return dernier_dossier;
    
}


void ecrire_fichier_dans_fichier_tmp(FILE * fich_tmp, char * chemin, char * nom) {

    FILE * fich = NULL;

    int c;

    fich = fopen(chemin, "r");
    printf("'ouverture du fichier '%s'\n", chemin);
    if (fich == NULL) {
        printf("Erreur d'ouverture du fichier '%s'\n", chemin);
        exit(EXIT_FAILURE);
    }

    fprintf(fich_tmp, "%s\n", nom);

    /* on écrit le contenu du fichier */
    while ((c = fgetc(fich)) != EOF) {
        fprintf(fich_tmp, "%c", c);
    }

    fprintf(fich_tmp, "%c%c%c", 1, 1, 1); /* séparateur */

    fclose(fich);
}



/* écrit les fichiers contenus dans un dossier dans le fichier temporaire fich_tmp */
void ecrire_fichiers_dossier_dans_fichier_tmp(FILE * fich_tmp, char * chemin_dossier) {
    DIR * d;
    struct dirent *dir;
    char dir_path[100];
    char file_name[100];
    char * chemin_copie = (char *) malloc (100 * sizeof(char)); 
    int i = 1;

    /* nom du dernier dossier */
    char nom_dossier[100];

    strcpy(chemin_copie, chemin_dossier);
    d = opendir(chemin_dossier);
        
    strcpy(nom_dossier, obtenir_nom_dernier_dossier(chemin_dossier));
    printf("nom dossier obtenu: %s\n", nom_dossier);
        
    if (d) {
        
        while ((dir = readdir(d)) != NULL) {
            /* exclure . et .. */
            if (strcmp(dir->d_name, ".") != 0 && strcmp(dir->d_name, "..") != 0) {

                /* recréer le chemin d'accès du fichier */
                /* printf("d_name: %s\n", d);*/
                strcpy(dir_path, chemin_copie);
                strcat(dir_path, "/");
                strcat(dir_path, dir->d_name);

                /* nom du fichier */
                strcpy(file_name, nom_dossier);
                strcat(file_name, "/");
                strcat(file_name, dir->d_name);
                
                if (!est_fichier(dir_path)) {
                    printf("erreur: %s est un dossier!\n", dir->d_name);
                    /* return; */
                }
                printf("fichier %d: %s\n", i, dir->d_name);

                ecrire_fichier_dans_fichier_tmp(fich_tmp, dir_path, file_name);
                
                i++;
            }
        }
        closedir(d);
        
    }
}



void compiler_dans_fichier_tmp(char * argv[], int argc) {
    
    int i_fich;
    int fichier_valide = 0;

    FILE * fich_tmp = NULL;

    fich_tmp = fopen("tmp.tmpcomp", "w+");
    if (fich_tmp == NULL) {
        printf("Erreur de création du fichier temporaire\n");
        exit(EXIT_FAILURE);
    }

    for (i_fich = 3; i_fich < argc; i_fich++) {

        fichier_valide = est_fichier(argv[i_fich]);
        /* si c'est un fichier */
        if (fichier_valide == 1) {

            ecrire_fichier_dans_fichier_tmp(fich_tmp, argv[i_fich], argv[i_fich]);

            printf("fichier '%s' bien traité\n", argv[i_fich]);
        }
        else {
            if (fichier_valide == 0) {
                printf("ouverture du dossier '%s'\n", argv[i_fich]);
                ecrire_fichiers_dossier_dans_fichier_tmp(fich_tmp, argv[i_fich]);
            }
            else {
                printf("erreur: le fichier '%s' n'a pas pu être traité (il n'existe probablement pas)\n", argv[i_fich]);
            }
        }
        
    }

    fclose(fich_tmp);
    
}


/* en supposant les arguments valides */
void compression_multifichiers(char * argv[], int argc) {

    FILE * fich_tmp = NULL;

    /* première étape: compilation de tous les fichiers dans un fichier temporaire */
    
    compiler_dans_fichier_tmp(argv, argc);
    
    printf("fin de compilation dans un fichier, début compression\n");

    /* deuxième étape: compression de ce fichier en un fichier compressé */

    fich_tmp = fopen("tmp.tmpcomp", "r");
    if (fich_tmp == NULL) {
        printf("Erreur d'ouverture du fichier temporaire après l'écriture\n");
        exit(EXIT_FAILURE);
    }

    boucle_compresse(fich_tmp, argv[2]);

    fclose(fich_tmp);

    /* supprimer le fichier temporaire */
    if (remove("tmp.tmpcomp") != 0) {
        printf("Erreur de suppression de 'tmp.tmpcomp'\n");
        exit(EXIT_FAILURE);
    }
    
}







/* reconstitue les fichiers de chemin_fich dans le dossier dossier */
void reconstituer_fichiers(char * chemin_fich, char * dossier) {

    FILE * fich = NULL;
    FILE * fich_part = NULL;

    char * line = NULL;
    char chemin_fich_part[100];
    size_t len = 0;
    int chemin_size;

    int loop = 1;
    int char_loop = 1;

    int c;

    fich = fopen(chemin_fich, "r");
    if (fich == NULL) {
        printf("erreur lors de l'ouverture du fichier\n");
        exit(EXIT_FAILURE);
    }


    
    /* si le dossier de destination n'existe pas */
    if (dossier != NULL && est_fichier(dossier) == -1) {
        printf("Erreur: le dossier '%s' n'existe pas\n", dossier);
        exit(EXIT_FAILURE);
        
        if (mkdir(dossier, 777)) {
            printf("Erreur de création du dossier '%s'\n", dossier);
            exit(EXIT_FAILURE);
        }
        
    }

    

    while (loop == 1) {
        printf("scan\n");
        
        /* obtenir le nom */
        /* fscanf(fich, "%[^\n]", line) == -1 */
        if (getline(&line, &len, fich) == -1) {
            /* on ne lit plus rien (fin du fichier): on sort de la boucle */
            loop = 0;
            break;
        }

        /* enlever le saut de ligne à la fin */
        chemin_size = strlen(line);
        line[ chemin_size - 1 ] = '\0';

        /* recréer le dossier */
        if (!mkdir(obtenir_nom_dossier(line))) {
            printf("Erreur de création du dossier\n");
            exit(EXIT_FAILURE);
        }

        strcpy(chemin_fich_part, "");
        if (dossier != NULL) {
            strcat(chemin_fich_part, dossier);
            strcat(chemin_fich_part, "/");
        }
        strcat(chemin_fich_part, line);
        
        printf("compression de '%s'\n", chemin_fich_part);

        /* créer le fichier avec le bon nom */
        /* vérifier que le fichier n'existe pas déjà? */
        
        fich_part = fopen(chemin_fich_part, "w");
        if (fich_part == NULL) {
            printf("erreur lors de la création du fichier %s\n", chemin_fich_part);
            exit(EXIT_FAILURE);
        }

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


void decompression_multifichiers(FILE * f, char * nom_fichier, char * chemin_dossier) {

    /* chemin du fichier décompressé */
    char chemin_decomp[100];
    
    /* décompresser l'archive */
    
    boucle_decompresse(f, nom_fichier, NULL); /* NULL car on décompresse l'archive en un fichier temporaire dans le répertoire actuel */

    printf("fin boucle décompresse\n");

    /* on crée le chemin du fichier qui a été décompressé */
    strcpy(chemin_decomp, nom_fichier);
    strcat(chemin_decomp, ".decomp");

    printf("fin refait fichier %s\n", chemin_decomp);

    /* reconstituer les fichiers (dans chemin_dossier si précisé) */
    reconstituer_fichiers(chemin_decomp, chemin_dossier);

    printf("fin reconstitue\n");

    /* supprimer le fichier temporaire */
    if (remove(chemin_decomp) != 0) {
        printf("Erreur de suppression de '%s'\n", chemin_decomp);
        exit(EXIT_FAILURE);
    }
}



/* ----------------------------------------------------------------------- */
/* FIN PLUSIEURS FICHIERS */
/* ----------------------------------------------------------------------- */




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
            fprintf(stderr, "Veuillez préciser un nom d'archive et une liste de fichiers ou dossiers à compresser\n");
            exit(EXIT_FAILURE);
        }
        
        if (argc < 4) {
            fprintf(stderr, "Veuillez préciser une liste de fichiers ou dossiers à compresser\n");
            exit(EXIT_FAILURE);
        }

        compression_multifichiers(argv, argc);

        /* appel de compression */
        printf("compr\n");
        
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

        if (argc >= 4) {
            chemin_dossier = argv[3];
        }
        decompression_multifichiers(fic, argv[2], chemin_dossier);

        fclose(fic);

        /* A ENLEVER */
        if (argc < 4) {
            printf("decomp sans dossier cible\n");
        }
        else {
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


    /* reconstituer_fichiers("tmp.tmpcomp"); */
    
    exit(EXIT_SUCCESS);
}