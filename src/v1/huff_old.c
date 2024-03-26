#include <stdio.h>
#include <stdlib.h>
#include <limits.h>

#include "arbre.h"

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

/* initialise tous les pointeurs du tableau à NULL */
void initialiser_arbre_huffman(noeud * arbre_huffman[256]) {
    int i;

    for (i = 0; i < 256; i++) {
        arbre_huffman[i] = NULL;
    }
}

void afficher_occurences(noeud * arbre_huffman[256]) {
    int i;
    char c;
    
    for (i = 0; i < 256; i++) {
        if (arbre_huffman[i] != NULL) {

            c = arbre_huffman[i]->c; /* caractère actuel */

            /* affichage spécial pour les caractère "invisibles" */
            if (c == '\n') printf("\\n");
            else if (c == ' ') printf("espace");
            else if (c == '\t') printf("tab");
            else printf("%c", c);

            /* afficher le nombre d'occurences */
            printf(" : %d\n", arbre_huffman[i]->occurence);
        }
    }
}

/* crée une structure noeud pour chaque caractère contenu dans le fichier */
void creer_noeuds_caracteres(int tab[256], noeud * arbre_huffman[256]) {
    int i;
    
    for (i = 0; i < 256; i++) {
        if (tab[i] != 0) {

            arbre_huffman[i] = creer_feuille(tab, i);
        }
    }
}

/* 4.2.6 */
/* place dans i1 et i2 les indices des deux noeuds aux occurences les plus petites (i1 <= i2) */
void deux_entiers_petits(noeud * arbre_huffman[256], int * i1, int * i2) {
    int i;
    int min = INT_MAX;
    
    *i2 = -1;
    *i1 = -1;

    for (i = 0; i < 256; i++) {
        if (arbre_huffman[i] != NULL && arbre_huffman[i]->occurence <= min) {
            *i2 = *i1; /* i2 prend l'ancienne valeur de i1 */
            *i1 = i; /* i1 devient l'indice du noeud avec le plus petit nombre d'occurences */
            
            min = arbre_huffman[i]->occurence;
        }
    }
}

/* 4.2.7 */
void creer_noeud(noeud * tab[], int taille) {
    int i1, i2;
    
    deux_entiers_petits(tab, &i1, &i2);
    
    printf("indice %d : %c, indice %d : %c\n", i1, tab[i1]->c, i2, tab[i2]->c);
}

void usage(char *s){
    printf("Usage %s : <fichier>\n", s);
}

int main(int argc, char *argv[]) {

    FILE * fic = NULL;
    int tab[256]; /* nombre d'occurences de chaque caractère */
    noeud * arbre_huffman[256]; /* pointeurs vers des noeuds */

    if (argc < 2){
        usage(argv[0]);
        exit(EXIT_FAILURE);
    }

    if ((fic = fopen(argv[1], "r")) == NULL){
       printf("Erreur d'ouverture du fichier %s\n", argv[1]);
       exit(EXIT_FAILURE);
    }

    initialiser_occurences(tab);
    initialiser_arbre_huffman(arbre_huffman);

    /* appel de la fonction occurence */
    occurence(fic, tab);

    /* 4.2.5 */
    creer_noeuds_caracteres(tab, arbre_huffman);

    /* on affiche les occurences de chaque caractère (si il y a eu une occurence) */
    afficher_occurences(arbre_huffman);

    /* TEST */
    creer_noeud(arbre_huffman, 256);
    /* FIN TEST */

    fclose(fic);

    exit(EXIT_SUCCESS);

}
