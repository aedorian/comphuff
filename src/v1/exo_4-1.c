#include <stdio.h>
#include <stdlib.h>

#define NB_CHAR 1  /* On lit caractère par caractère */


int main(){

    char c[NB_CHAR];   /* buffer contenant le caractère lu */
    FILE* fich = NULL;
    size_t taille_carac = sizeof(char);   /* taille d'un caractère dans le fichier */

    if ((fich = fopen("fichier.txt", "rb")) == NULL){   /* ouverture du fichier en lecture binaire */
        printf("Erreur d'ouverture du fichier fichier.txt\n");
    }

    /* lecture jusqu'à ce que ce ne soit plus possible, et affichage du caractère */
    /* c : buffer, taille_carac : taille d'un caractère, NB_CHAR : lecture d'un caractère */
    while (fread(c, taille_carac, NB_CHAR, fich) != 0){
        printf("%s", c);
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
