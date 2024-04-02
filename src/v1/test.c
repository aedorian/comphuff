#include <stdio.h>
#include <stdlib.h>

#define CHAR 0
#define TAILLE 1
#define CODE 2

int puiss_2(int x){
    if (x == 0){
        return 1;
    }
    return puiss_2(x - 1) * 2;
}

/* remplit buffer_c des chiffres du caractère en binaire */
void char2bin(char c, int buffer_c[8]) {
    int i;
    for (i = 7; i >= 0; i--) {
        buffer_c[i] = c % 2;
        c = c >> 1;
    }
}

/* convertit un buffer de int en un entier */
int bin2int(int buffer_r[8], int taille) {
    int i;
    int res = 0;
    for (i = taille-1; i >= 0; i--) {
        if (buffer_r[i] == 1) {
            res += puiss_2(i);
        }
    }

    return res;
}

void boucle_alphabet() {
    FILE * f = NULL;
    
    /* à lire au début (des int) */
    char nb_chars = 0; /* nombre de caractères différents */
    char depassement = 0; /* dans le dernier octet */
    
    int a_obtenir = CHAR;
    int taille_code = 0; /* taille du codage */
    /* suite de: CHAR (8 bits) - TAILLE (3 bits) - CODE (TAILLE (1 à 8 bits)) */

    int i;
    char c; /* utilisé pour lire 8 bits dans le fichier compressé */
    int buffer_c[8]; /* "pelleteuse": c mais en tableau de int */
    int buffer_r[8]; /* buffer de lecture */
    int i_r = 0; /* position actuelle dans buffer_r */
    int lu;

    f = fopen("banane.txt.comphuff", "r");
    if (f == NULL) printf("fuck");

    /* on lit les deux premières valeurs (sur 8 bits) */
    if (fscanf(f, "%c", &depassement) != 1) printf("ereeo");
    if (fscanf(f, "%c", &nb_chars) != 1) printf("erore");

    printf("depasse: %d\nnb_chars: %d\n", depassement, nb_chars);

    /* on lit l'alphabet entier (il y a nb_chars caractères différents) */
    for (i = 0; i < nb_chars; i++) {
        /* on vide le buffer 1 à 1 */
        for (i = 0; i < 8; i++) {
            /* on ajoute le bit lu dans buffer_c à la fin de buffer_r*/
            buffer_r[i_r] = buffer_c[i];
            i_r++;

            /* puis on regarde ce qu'on veut lire */
            lu = -1;
            switch (a_obtenir) {
            case CHAR:
                /* si on doit lire un caractère */
                if (i_r == 8) {
                    lu = bin2int(buffer_r, 8);
                    i_r = 0; /* on revient au début du buffer */
                    a_obtenir = TAILLE;
                }
                break;
            case TAILLE:
                /* si on doit lire un caractère */
                if (i_r == 3) {
                    lu = bin2int(buffer_r, 3);
                    i_r = 0;
                    a_obtenir = CODE;
                }
                break;
            case CODE:
                if (i_r == taille_code) {
                    lu = bin2int(buffer_r, taille_code);
                    i_r = 0;
                    a_obtenir = CHAR;
                }
                break;
            default: printf("wtf?\n"); break;
            }

            if (lu != -1) {
                printf("lu: %d\n", lu);
            }
        }
    }
}

int main() {
    int b_c[8];

    char c = 'E';

    int i = 0;

    char2bin(c, b_c);

    printf("%c\n", c);
    for (i = 0; i < 8; i++) {
        printf("%d", b_c[i]);
    }

    printf("\n");
    boucle_alphabet();

    exit(EXIT_SUCCESS);
}