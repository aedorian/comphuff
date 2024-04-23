#ifndef _UTILS_H_
#define _UTILS_H_

/* Permet de calculer 2^x.
Retourne le résultat. */
int puiss_2(int x);

/* Remplit buffer_c des chiffres du caractère en binaire. */
void char2bin(char c, int buffer_c[8]);

/* Convertit un buffer de int (de taille : taille) en un entier,
et le retourne. */
int bin2int(int buffer_r[8], int taille);

/* Convertit n en une chaîne de nb_bits caractères, en binaire,
et le retourne. */
/* ex: si n = 2 et nb_bits = 3, renvoie "010" */
char * int2string(int n, int nb_bits);

/* Calcule la taille de s,
Et la retourne. */
int taille(char *s);

/* vérifie si le fichier est un dossier ou un fichier "normal" */
/* 1 si c'est un fichier normal, 0 si ce n'est pas un fichier normal (donc un dossier dans notre cas) */
int est_fichier(char * chemin);

/* renvoie le nom du fichier (sans le chemin) */
char * nom_fich_base(char * chemin);

#endif /* _UTILS_H_ */