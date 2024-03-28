#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "arbre.h"

/* renvoie 2^x */
int puiss_2(int x){
    if (x == 0){
        return 1;
    }
    return puiss_2(x - 1) * 2;
}


/* écrit dans le fichier fic, à l'aide du buffer */
char ecrire_fich(FILE * fic, int *it, char buffer, int code, int start){
    int i;
    
    for (i = start; i >= 0; i--){
        if (*it == 8){
            *it = 0;
            /* écriture du buffer dans le fichier */
            fwrite(&buffer, sizeof(char), 1, fic);
            /* on vide le buffer */
            buffer = 0;
        }
        buffer = buffer << 1;
        if (code / puiss_2(i) == 1){
            buffer += 1;
        }
        code = code % puiss_2(i);
        *it += 1;
    }
    return buffer;
}


void creer_compresse(char * nom_fichier, FILE* fic, int nb_char, noeud * alphabet[256]){
    FILE * comp = NULL;
    char nom_comp[89]; /* nom du fichier compressé ###### (taille max de nom_fichier : 80)*/
    char buffer, c, depassement;
    int code, taille, it, i;
    
    /* On ce place au début du fichier à compresser */
    rewind(fic);
    

    /* création du fichier à compresser */
    strcpy(nom_comp, nom_fichier);
    strcat(nom_comp, ".comphuff");
    
    comp = fopen(nom_comp, "wb");
    if (fic == NULL) {
        fprintf(stderr, "Erreur main: erreur lors de l'ouverture du fichier \"%s\"\n", nom_fichier);
        exit(EXIT_FAILURE);
    }


    /* écriture de l'en-tête */
    /* écriture du nombre de caractère différent */
    c = nb_char;
    fwrite(&c, sizeof(char), 1, comp);

    /* écriture de la structure alphabet : [(char)(nb_bits)(codage)] * nb_char */
    buffer = 0;
    it = 0;
    for (i = 0; i < 256; i++){
        if (alphabet[i] -> codage != -1){
            /* (char) */
            buffer = ecrire_fich(comp, &it, buffer, i, 7); /* on écrit 8 bits */
            /* (nb_bits) */
            taille = alphabet[i] -> nb_bits;
            buffer = ecrire_fich(comp, &it, buffer, taille, 2);  /* on écrit 3 bits */
            /* (codage) */
            code = alphabet[i] -> codage;
            buffer = ecrire_fich(comp, &it, buffer, code, taille - 1);
        }
    }

    /* écriture du contenu */
    /* parcours caractère par caractère du fichier à compresser */
    it = 0;

    while ((c = fgetc(fic)) != EOF){
        code = alphabet[(int)c] -> codage;
        taille = alphabet[(int)c] -> nb_bits;
        buffer = ecrire_fich(comp, &it, buffer, code, taille - 1);
    }

    /* décalage suplémentaire et écriture du reste */
    depassement = 0;
    if (i != 0){
        while (i < 8){
            buffer = buffer << 1;
            i++;
            depassement++;
        }
        /* écriture du buffer dans le fichier */
        fwrite(&buffer, sizeof(char), 1, comp);
    }

    /* écriture au début du fichier du dépacement */
    rewind(fic);
    fwrite(&depassement, sizeof(char), 1, comp);
    
    fclose(comp);
}


int main(){


    exit(EXIT_SUCCESS);
}