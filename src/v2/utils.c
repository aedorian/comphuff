#include <stdio.h>
#include <stdlib.h>
#include <MLV/MLV_all.h>

#include "headers/utils.h"


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