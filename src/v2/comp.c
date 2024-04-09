#include <stdio.h>
#include <stdlib.h>
#include <limits.h> /* INT_MAX */
#include <MLV/MLV_all.h>

#include "headers/arbre.h"
#include "headers/comp.h"
#include "headers/utils.h"


/* Compte le nombre d'occurences de chaque caractère dans le fichier fic et place le résultat dans un tableau. */
void occurence(FILE *fic, int tab[256]) {
    char c;
    
    while ((c = fgetc(fic)) != EOF) {
        /* printf("%c", c); */
        tab[(int)c] += 1;
    }
}

/* Initialise le nombre d'occurences de chaque caractère à 0. */
void initialiser_occurences(int tab[256]) {
    int i;

    /* initialisation du tableau */
    for (i = 0; i < 256; i++) {
        tab[i] = 0;
    }

}

/* Initialise tous les pointeurs du tableau arbre_huffman à NULL. */
void initialiser_arbre_huffman(noeud * arbre_huffman[256]) {
    int i;

    for (i = 0; i < 256; i++) {
        arbre_huffman[i] = NULL;
    }
}

/* Affiche toute les occurences des caractères contenu dans le fichier. */
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

/* 4.2.4 */
/* Crée une structure noeud pour chaque caractère contenu dans le fichier.
Retourne la taille du tableau. */
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
/* Place dans i1 et i2 les indices des deux derniers noeuds aux occurences les plus petites (i1 <= i2). */
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


/* 4.2.7 */
/* ########### */
void creer_noeud(noeud * tab[], int taille) {
    int i1, i2;
    noeud * new;
    
    deux_entiers_petits(tab, &i1, &i2);
    
    printf("indice %d, indice %d\n", i1,  i2);

    /* check i1 et i2 != -1 ?  ########  */

    /* création du nouveau noeud */

    new = (noeud *) malloc (sizeof(noeud));
    if (new == NULL) {
        fprintf(stderr, "Erreur creer_noeud: erreur d'allocation mémoire\n");
        exit(EXIT_FAILURE);
    }

    /* TROUVER LA POSITION NULL LA PLUS PROCHE?  ######### */

    new->c = 0;
    new->occurence = tab[i1]->occurence + tab[i2]->occurence;
    new->codage = 0;
    new->nb_bits = 0;

    new->gauche = tab[i1];
    new->droit = tab[i2];

    tab[i1] = new;
    tab[i2] = NULL;
}


/* Affiche le codage du caractère (int) en binaire. */
void affichage_code(int nbr_bits, int codage){
    if (nbr_bits <= 0){
        return;
    }

    affichage_code(nbr_bits - 1, codage / 2);
    /* affichage après l'appel récursif pour l'afficher à l'endroit */
    printf("%d", codage % 2);
}


/* Stocke dans la structure alphabet le codage des caractères. */
void creer_code(noeud * element, int code, int profondeur, noeud * alphabet[256]){
    if (est_feuille(element)){
        element -> codage = code;
        element -> nb_bits = profondeur;
        /* rajouté */
        
        affichage_code(profondeur, code);
        printf("(%c)-", element->c);
        /* stockage dans la structure alphabet */
        alphabet[(int)element -> c] = element;
        printf("%c\n", alphabet[(int) element -> c]->c);
    }
    else {
        creer_code(element -> gauche, code * 2, profondeur + 1, alphabet);
        creer_code(element -> droit, code * 2 + 1, profondeur + 1, alphabet);
    }
}


/* Écrit dans le fichier fic, à l'aide du buffer. Renvoi le buffer qui sera probablement non vide. */
/* Start doit être n - 1. */
char ecrire_fich(FILE * fic, int *it, char buffer, int code, int start){
    int i;
    
    for (i = start; i >= 0; i--){
        /* écriture seulement si le buffer est plein */
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

/* Crée et écrit dans le fichier compresser avec l'extension .comphuff
le fichier sera de la forme : dépacement, nombre de caractères différent, structure alphabet, contenu compressé. */
void creer_compresse(char * nom_fichier, FILE* fic, int nb_char, noeud * alphabet[256]){
    FILE * comp = NULL;
    char nom_comp[89]; /* nom du fichier compressé ###### (taille max de nom_fichier : 80)*/
    char buffer, c, depassement; /* 1 char = 8 bits */
    int code, taille, it, i;
    
    /* on se place au début du fichier à compresser */
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
    /* on écrit un emplacement à remplacer par le rewind de la fin pour le dépassement */
    c = 0;
    fwrite(&c, sizeof(char), 1, comp);
    
    /* écriture du nombre de caractère différent */
    printf("IL Y A %d\n", nb_char);
    c = (char)nb_char;
    printf("IL Y A %c %d\n", c, c);
    fwrite(&c, sizeof(char), 1, comp);

    /* écriture de la structure alphabet : [(char)(nb_bits)(codage)] * nb_char */
    buffer = 0;
    it = 0;
    for (i = 0; i < 256; i++){
        if (alphabet[i] != NULL) {
            if (alphabet[i] -> codage != -1){
                /* (char) */
                printf("%c %d\n", alphabet[i]->c, alphabet[i]->codage);
                buffer = ecrire_fich(comp, &it, buffer, i, 7); /* on écrit 8 bits */
                /* (nb_bits) */
                printf("%d \n", alphabet[i]->nb_bits);
                taille = alphabet[i] -> nb_bits;
                buffer = ecrire_fich(comp, &it, buffer, taille - 1, 2);  /* on écrit 3 bits */
                /* (codage) */
                code = alphabet[i] -> codage;
                buffer = ecrire_fich(comp, &it, buffer, code, taille - 1);
            }
        }
    }

    /* écriture du contenu */
    /* parcours caractère par caractère du fichier à compresser */
    /* it = 0; */

    while ((c = fgetc(fic)) != EOF){
        code = alphabet[(int)c] -> codage;
        taille = alphabet[(int)c] -> nb_bits;
        buffer = ecrire_fich(comp, &it, buffer, code, taille - 1);
    }

    /* décalage suplémentaire et écriture du reste */
    i = it;
    
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
    
    rewind(comp);   /* Retour au début du fichier */
    printf("depassement: %d\n", depassement);
    fwrite(&depassement, sizeof(char), 1, comp);
    
    fclose(comp);
}
