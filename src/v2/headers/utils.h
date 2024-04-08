#ifndef _UTILS_H_
#define _UTILS_H_

/* retourne 2^x */
int puiss_2(int x);

/* remplit buffer_c des chiffres du caractère en binaire */
void char2bin(char c, int buffer_c[8]);

/* convertit un buffer de int (de taille taille) en un entier */
int bin2int(int buffer_r[8], int taille);

/* convertit n en une chaîne de nb_bits caractères ('0' ou '1') et retourne cette chaîne*/
/* ex: si n = 2 et nb_bits = 3, renvoie "010" */
char * int2string(int n, int nb_bits);

#endif /* _UTILS_H_ */