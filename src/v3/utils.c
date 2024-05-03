#include <stdio.h>
#include <stdlib.h>
#include <MLV/MLV_all.h>

#include <sys/types.h>
#include <sys/stat.h>
#include <unistd.h>
#include <libgen.h>

#include "headers/utils.h"
#include "headers/arbre.h"
#include "headers/comp.h"

/* Permet de calculer 2^x.
Retourne le résultat. */
int puiss_2(int x){
    return 1 << x;
}

/* Remplit buffer_c des chiffres du caractère en binaire. */
void char2bin(char c, int buffer_c[8]) {
    int i;
    for (i = 7; i >= 0; i--) {
        buffer_c[i] = (unsigned int)c % 2;
        c = c >> 1;
    }
}

/* Convertit un buffer de int (de taille : taille) en un entier,
et le retourne. */
int bin2int(int buffer_r[8], int taille) {
    int i;
    int res = 0;
    for (i = taille-1; i >= 0; i--) { /* on parcourt le tableau de droite à gauche */
        if (buffer_r[i] == 1) {
            res += puiss_2(taille - 1 - i);
        }
    }

    return res;
}

/* Convertit n en une chaîne de nb_bits caractères, en binaire,
et le retourne. */
/* ex: si n = 2 et nb_bits = 3, renvoie "010" */
char * int2string(int n, int nb_bits) {
    char * codage;
    int i;

    /* nb_bits + 1 car c'est la taille - 1 du codage qui est stocké */
    codage = (char *) malloc ((nb_bits + 1) * sizeof(char));

    for (i = 0; i < nb_bits; i++) {
        codage[nb_bits - 1 - i] = (n % 2 == 0) ? '0' : '1';
        n /= 2;
    }
    codage[nb_bits] = '\0';

    return codage;
}

/* Calcule la taille de s,
Et la retourne. */
int taille(char *s){
    char *chaine = s;
    int t = 0;

    while (chaine[0] != '\0'){
        t++;
        chaine++;
    }

    return t;
}

/* vérifie si le fichier est un dossier ou un fichier "normal" */
/* 1 si c'est un fichier normal, 0 si ce n'est pas un fichier normal (donc un dossier dans notre cas) */
/* renvoie aussi -1 si le fichier ou dossier n'existe pas */
int est_fichier(char * chemin) {
    struct stat chemin_stat;
    if (stat(chemin, &chemin_stat)) {
        /* printf("est_fichier(): erreur de la fonction stat()\n"); */
        return -1;
    }
    return S_ISREG(chemin_stat.st_mode);
}

char * nom_fich_base(char * chemin) {
    return basename(chemin);
}

void debug_alphabet(noeud * a[256]) {
    int i;

    for (i = 0; i < 256; i++) {
        printf("%d\n", i);
        if (a[i] != NULL) {
            printf("a[%d] : ", i);
            printf("%c ", a[i]->c);
            affichage_code(a[i]->nb_bits, a[i]->codage);
            printf("\n");
        }
    }
}
