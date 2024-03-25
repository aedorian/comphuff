#include <stdio.h>
#include <stdlib.h>

/* compte le nombre d'occurences de chaque caractère dans le fichier fic
   et place le résultat dans un tableau */
void occurence(FILE *fic, int tab[256]) {
    char c;
    
    while ((c = fgetc(fic)) != EOF) {
        /* printf("%c", c); */
        tab[(int)c] += 1;
    }
}

int main() {

    FILE * fic;
    int tab[256];
    int i;

    fic = fopen("huff_v1.c", "r");

    /* initialisation du tableau */
    for (i = 0; i < 256; i++) {
        tab[i] = 0;
    }

    /* appel de la fonction occurence */
    
    occurence(fic, tab);

    /* on affiche les occurences de chaque caractère (si il y a eu une occurence) */
    for (i = 0; i < 256; i++) {
        if (tab[i] != 0) {

            /* affichage spécial pour les caractère "invisibles" */
            if (i == '\n') printf("\\n");
            else if (i == ' ') printf("espace");
            else if (i == '\t') printf("tab");
            else printf("%c", i);

            printf(" : %d\n", tab[i]);
        }
    }

    fclose(fic);

    exit(EXIT_SUCCESS);

}