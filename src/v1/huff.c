#include <stdio.h>
#include <stdlib.h>
#include <limits.h>
#include <string.h>
#include <MLV/MLV_all.h>

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
            affiche_car(c);

            /* afficher le nombre d'occurences */
            printf(" : %d\n", arbre_huffman[i]->occurence);
        }
    }
}

/* 4.2.4 crée une structure noeud pour chaque caractère contenu dans le fichier */
int creer_noeuds_caracteres(int tab[256], noeud * arbre_huffman[256]) {
    int i;
    int i_huffman = 0; /* index dans arbre_huffman */
    
    for (i = 0; i < 256; i++) {
        if (tab[i] != 0) {
            
            arbre_huffman[i_huffman] = creer_feuille(tab, i);;
            i_huffman++;
        }
    }

    return i_huffman; /* taille du tableau */
}

/* 4.2.6 */
/* place dans i1 et i2 les indices des deux noeuds aux occurences les plus petites (i1 <= i2) */
void deux_entiers_petits(noeud * arbre_huffman[256], int * i1, int * i2) {
    int i;
    int min = INT_MAX;

    *i2 = -1;
    *i1 = -1;

    for (i = 0; i < 256; i++) {
        if (arbre_huffman[i] != NULL) printf("%d ", arbre_huffman[i]->occurence);
        
        if (arbre_huffman[i] != NULL && arbre_huffman[i]->occurence <= min) {
            *i2 = *i1; /* i2 prend l'ancienne valeur de i1 */
            *i1 = i; /* i1 devient l'indice du noeud avec le plus petit nombre d'occurences */
            
            min = arbre_huffman[i]->occurence;
        }
    }

    /* si il n'y a plus que deux éléments, et que occ(dernier élément) > occ(arbre_huffman[*i1]) */
    if (*i2 == -1) {
        for (i = *i1; i < 256; i++) {
            if (arbre_huffman[i] != NULL && i != *i1) {
                *i2 = i; /* i2 prend l'ancienne valeur de i1 */
                break;
            }
        }
    }
}

void debug_huffman(noeud * huff[], int taille) {
    int i;

    printf("---------------- début debug ---------------------------------------\n");

    for (i = 0; i < taille; i++) {

        /*
        printf("%d : ", i);
        if (huff[i] == NULL) printf("-\n");
        else printf("%c (%d)\n", huff[i]->c, huff[i]->occurence);
        */

        afficher_arbre(huff[i]);
    }

    printf("---------------- fin debug ---------------------------------------\n");
}

/* 4.2.7 */
void creer_noeud(noeud * tab[], int taille) {
    int i1, i2;
    noeud * new;
    
    deux_entiers_petits(tab, &i1, &i2);
    
    printf("indice %d, indice %d\n", i1,  i2);

    /* check i1 et i2 != -1 ? */

    /* création du nouveau noeud */

    new = (noeud *) malloc (sizeof(noeud));
    if (new == NULL) {
        fprintf(stderr, "Erreur creer_noeud: erreur d'allocation mémoire\n");
        exit(EXIT_FAILURE);
    }

    /* TROUVER LA POSITION NULL LA PLUS PROCHE? */

    /* pas sûr pour la définition des membres */
    new->c = 0; /* ??? */
    new->occurence = tab[i1]->occurence + tab[i2]->occurence;
    new->codage = 0; /* ??????? */
    new->nb_bits = 0; /* pas encore défini */

    new->gauche = tab[i1];
    new->droit = tab[i2];

    tab[i1] = new;
    tab[i2] = NULL;
}









void affichage_code(int nbr_bits, int codage){
    if (nbr_bits <= 0){
        return;
    }

    affichage_code(nbr_bits - 1, codage / 2);
    /* affichage après l'appel récursif pour l'afficher à l'endroit */
    printf("%d", codage % 2);
}


/* mettre 0 à code */
void creer_code(noeud * element, int code, int profondeur, noeud * alphabet[256]){
    if (est_feuille(element)){
        element -> codage = code;
        element -> nb_bits = profondeur;
        
        affichage_code(profondeur, code);
        printf("(%c)-", element->c);
        /* stockage dans la structure alphabet */
        alphabet[(int)element -> c] = element;
    }
    else {
        creer_code(element -> gauche, code * 2, profondeur + 1, alphabet);
        creer_code(element -> droit, code * 2 + 1, profondeur + 1, alphabet);
    }
}





/* à modulariser */
void creer_compresse(char * nom_fichier, int nb_char, noeud * alphabet[256]) {
    FILE * fic = NULL;
    FILE * comp = NULL;
    char nom_comp[89]; /* nom du fichier compressé */

    /* ouverture du fichier à compresser */
    fic = fopen(nom_fichier, "r");
    if (fic == NULL) {
        fprintf(stderr, "Erreur main: erreur lors de l'ouverture du fichier \"%s\"\n", nom_fichier);
        exit(EXIT_FAILURE);
    }

    

    /* création du fichier à compresser */
    strcpy(nom_comp, nom_fichier);
    strcat(nom_comp, ".comphuff");
    
    comp = fopen(nom_comp, "wb");
    if (fic == NULL) {
        fprintf(stderr, "Erreur main: erreur lors de l'ouverture du fichier \"%s\"\n", nom_fichier);
        exit(EXIT_FAILURE);
    }

    fclose(fic);
    fclose(comp);
}







void usage(char *s){
    printf("Usage %s : <fichier>\n", s);
}

int main(int argc, char *argv[]) {

    FILE * fic = NULL;
    int tab[256]; /* nombre d'occurences de chaque caractère */
    noeud * arbre_huffman[256]; /* pointeurs vers des noeuds */
    int n_huffman; /* taille du tableau arbre_huffman = nombre de feuilles à l'origine = nombre de caractères */

    int n;

    noeud * alphabet[256];

    if (argc < 2){
        usage(argv[0]);
        exit(EXIT_FAILURE);
    }

    fic = fopen(argv[1], "r");
    if (fic == NULL) {
        fprintf(stderr, "Erreur main: erreur lors de l'ouverture du fichier \"%s\"\n", argv[1]);
        exit(EXIT_FAILURE);
    }

    initialiser_occurences(tab);
    initialiser_arbre_huffman(arbre_huffman);

    /* appel de la fonction occurence */
    occurence(fic, tab);
    
    /* 4.2.5 */
    n_huffman = creer_noeuds_caracteres(tab, arbre_huffman);

    /* on affiche les occurences de chaque caractère (si il y a eu une occurence) */
    afficher_occurences(arbre_huffman);

    debug_huffman(arbre_huffman, n_huffman);

    /* on crée l'arbre */
    n = n_huffman;
    while (n != 1) {

        creer_noeud(arbre_huffman, 256);

        n--;

        debug_huffman(arbre_huffman, n_huffman);
    }

    printf("fin de création des noeuds\n");

    /* trouver l'arbre final dans la structure */
    n = 0;
    while (arbre_huffman[n] == NULL) n++;
    /* le premier pointeur est l'arbre final */
    arbre_huffman[0] = arbre_huffman[n];

    /* créer l'alphabet */
    creer_code(arbre_huffman[0], 0, 0, alphabet);

    /* créer le fichier compressé */
    /* ATTENTION POUR ETENDRE LES OPTIONS */
    creer_compresse(argv[1], fic, n_huffman, alphabet);

    /* on ferme le fichier */
    fclose(fic);
    
    afficher_arbre_graphique(arbre_huffman[0]);

    exit(EXIT_SUCCESS);

}
