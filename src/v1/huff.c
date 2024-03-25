#include <stdio.h>
#include <stdlib.h>

/* compte le nombre d'occurences de chaque caractère dans le fichier fic
   et place le résultat dans un tableau */
/* sans fread(), problème? */
void occurence(FILE *fic, int tab[256]) {
    char c;
    
    while ((c = fgetc(fic)) != EOF) {
        /* printf("%c", c); */
        tab[(int)c] += 1;
    }
}

/* initialise le nombre d'occurences de chaque caractère à 0 */
void initialiser_occurences(int tab[256]) {
    int i;

    /* initialisation du tableau */
    for (i = 0; i < 256; i++) {
        tab[i] = 0;
    }

}

void afficher_occurences(int tab[256]) {
    int i;
    
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

}

void usage(char *s){
    printf("Usage %s : <fichier>\n", s);
}

int main(int argc, char *argv[]) {

    FILE * fic = NULL;
    int tab[256]; /* nombre d'occurences de chaque caractère */

    if (argc < 2){
        usage(argv[0]);
        exit(EXIT_FAILURE);
    }

    fic = fopen(argv[1], "r");

    initialiser_occurences(tab);

    /* appel de la fonction occurence */
    occurence(fic, tab);

    /* on affiche les occurences de chaque caractère (si il y a eu une occurence) */
    afficher_occurences(tab);

    fclose(fic);

    exit(EXIT_SUCCESS);

}