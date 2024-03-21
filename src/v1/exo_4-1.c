#include <stdio.h>
#include <stdlib.h>

#define NB_CHAR 1


void usage(char *s){
    printf("Usage %s : <fichier> <entier>, <entier> facultatif -> nombre de caractère lu", s);
}


int main(int argc, char *argv[]){

    char c[NB_CHAR];   /* buffer contenant le caractère lu */
    FILE* fich = NULL;
    size_t taille_carac = sizeof(char);   /* taille d'un caractère dans le fichier */
    int taille;

    if (argc < 2){
        usage(argv[0]);
        exit(EXIT_FAILURE);
    }
    if (argc == 2){
        taille = 10;
    }
    else {
        taille = atoi(argv[2]);
    }

    if ((fich = fopen(argv[1], "rb")) == NULL){   /* ouverture du fichier en lecture binaire */
        printf("Erreur d'ouverture du fichier fichier.txt\n");
    }

    /* ??? */
    /* c : buffer, taille_carac : taille d'un caractère, NB_CHAR : lecture de "taille" caractères */
    while (fread(c, taille_carac, NB_CHAR, fich) != 0 && taille > 0){
        printf("%s", c);
        taille--;
    }

    /*  QESTION 2 
    FILE* fich = NULL;
    int c;
    
    
    / * ouverture du fichier * /
    if ((fich = fopen("fichier.txt", "r")) == NULL){
        printf("Erreur d'ouverture du fichier fichier.txt\n");
        exit(EXIT_FAILURE);
    }

    / * lecture du fichier caractère par caractère, et affichage du caractère * /
    while ((c = fgetc(fich)) != EOF){
        printf("%c", c);
    }
    */

    fclose(fich);

    exit(EXIT_SUCCESS);
}
