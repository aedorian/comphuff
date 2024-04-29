#include <stdio.h>
#include <stdlib.h>
#include <string.h>


void gen_fich(int nb_car, int nb_car_diff, int graine, char * nom){
    FILE* fic = NULL;
    int i;
    
    if (strcmp(nom, "") != 0){    /* nom de fichier précisé */
        if ( (fic = fopen(nom, "w")) == NULL ){
            printf("Erreur ouverture / creation du fichier %s\n", nom);
            exit(EXIT_FAILURE);
        }
    }
    else {   /* pas de nom */
        if ( (fic = fopen("gen_fich.txt", "w")) == NULL ){
            printf("Erreur ouverture / creation du fichier gen_fich.txt\n");
            exit(EXIT_FAILURE);
        }
    }

    srand(graine);
    
    for (i = 0; i < nb_car; i++){
        fputc(rand() % nb_car_diff, fic);
    }

    printf("Fichier généré\n");
    
    fclose(fic);
}
