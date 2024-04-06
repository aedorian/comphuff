#include <stdio.h>
#include <stdlib.h>
#include <MLV/MLV_all.h>

#include "arbre.h"

#define CHAR 0
#define TAILLE 1
#define CODE 2

noeud * creer_huffman_inverse(noeud * alphabet[256]);

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
        buffer_c[i] = (unsigned int)c % 2;
        c = c >> 1;
    }
}

/* convertit un buffer de int en un entier */
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

/* convertit n en une chaîne de nb_bits caractères, en binaire */
char * int2string(int n, int nb_bits) {
    char * codage;
    int i;
    
    codage = (char *) malloc ((nb_bits + 1) * sizeof(char));

    for (i = 0; i < nb_bits; i++) {
        codage[nb_bits - 1 - i] = (n % 2 == 0) ? '0' : '1';
        n /= 2;
    }
    codage[nb_bits] = '\0';

    return codage;
}


noeud * lire_bit_arbre(noeud * arbre_huffman, noeud * parcours, int bit) {
    if (bit == 0) {
        parcours = parcours->gauche;
    } else parcours = parcours->droit;
    /* parcours = (bit == 0) ? parcours->gauche : parcours->droit;*/
    if (parcours->gauche == NULL && parcours->droit == NULL) {
        printf("%c", parcours->c);
        parcours = arbre_huffman; /* retour au début */
    }

    return parcours;
}



/* void lire_fich(FILE * fic, int */

/* cette fonction lit l'entête du fichier en paramètre */
noeud * boucle_decomp(FILE * f) {
    noeud * alphabet[256];
    
    /* à lire au début (des int) */
    char nb_chars = 0; /* nombre de caractères différents */
    char depassement = 0; /* dans le dernier octet */
    
    int a_obtenir = CHAR;
    int taille_code = 0; /* taille du codage */
    /* suite de: CHAR (8 bits) - TAILLE (3 bits) - CODE (TAILLE (1 à 8 bits)) */

    int nb_chars_lus; /* nombre de caractères lus */
    int j;
    char c; /* utilisé pour lire 8 bits dans le fichier compressé */
    int buffer_c[8]; /* "pelleteuse": c mais en tableau de int */
    int buffer_r[8]; /* buffer de lecture */
    int i_r; /* position actuelle dans buffer_r */
    
    int char_lu; /* le caractère en train d'être lu. aussi utilisé pour indexer alphabet */
    int lu; /* OBSOLETE */

    /* PARTIE HUFFMAN */
    noeud * arbre_huffman;

    /* initialisation de l'alphabet */
    for (j = 0; j < 256; j++) alphabet[j] = NULL;

    /* on lit les deux premières valeurs (sur 8 bits) */
    if (fscanf(f, "%c", &depassement) != 1) printf("ereeo");
    if (fscanf(f, "%c", &nb_chars) != 1) printf("erore");

    printf("depasse: %d\nnb_chars: %d\n", depassement, nb_chars);

    /* on lit l'alphabet entier (il y a nb_chars caractères différents) */
    nb_chars_lus = 0;
    i_r = 0;
    while (nb_chars_lus < nb_chars) {
        printf("---------- on lit un char -----------------------------------------------\n");
        /* on lit 8 bits dans c */
        if (fscanf(f, "%c", &c) != 1) printf("GERER ERREUR\n");
        /* on convertit les 8 bits en un tableau de int (buffer_c) */
        char2bin(c, buffer_c);

        /* on vide buffer_c 1 à 1 */
        for (j = 0; j < 8 && nb_chars_lus < nb_chars; j++) {
            /* on ajoute le bit lu dans buffer_c à la fin de buffer_r */
            buffer_r[i_r] = buffer_c[j];
            i_r++;

            /* puis on regarde ce qu'on veut lire, et on regarde si buffer_r est plein (8, 3 ou taille_code caractères) */
            lu = -1;
            switch (a_obtenir) {
            case CHAR:
                /* si on doit lire un caractère (encodé sur 8 bits) */
                if (i_r == 8) {
                    lu = bin2int(buffer_r, 8);
                    char_lu = lu;
                    i_r = 0; /* on revient au début du buffer */
                    a_obtenir = TAILLE;
                }
                break;
            case TAILLE:
                /* si on doit lire la taille (encodée sur 3 bits) */
                if (i_r == 3) {
                    lu = bin2int(buffer_r, 3) + 1; /* on a lu (taille_code - 1) dans le fichier compressé */
                    taille_code = lu;
                    i_r = 0;
                    a_obtenir = CODE;
                }
                break;
            case CODE:
                /* si on doit lire un code (encodé sur taille_code bits, récupérée précédemment) */
                if (i_r == taille_code) {
                    lu = bin2int(buffer_r, taille_code);
                    i_r = 0;
                    a_obtenir = CHAR;
                    
                    /* on a maintenant lu les 3 données (char, taille code, code) d'un caractère */
                    /* on peut créer un noeud dans alphabet */
                    alphabet[char_lu] = (noeud *) malloc (sizeof(noeud));
                    if (alphabet[char_lu] == NULL) {
                        fprintf(stderr, "Erreur boucle_entete: erreur d'allocation mémoire\n");
                        exit(EXIT_FAILURE);
                    }
                    alphabet[char_lu]->c = char_lu;
                    alphabet[char_lu]->codage = lu;
                    alphabet[char_lu]->nb_bits = taille_code;
                    
                    nb_chars_lus++;
                }
                break;
            default: break;
            }
        }
    }


    /* FIN DE LECTURE DE L'ALPHABET */

    /* on ne ferme pas le fichier, on l'utilisera plus tard */
    
    arbre_huffman = creer_huffman_inverse(alphabet);

    /* ECRITURE DU FICHIER */
    
    noeud * parcours = arbre_huffman;
    char c_next = 0; /* prochain octet, initialisé à 0 pour NULL */
    int loop = 1;

    /* finir de vider le buffer restant */
    printf("restant: %d\n", j);
    while (j != 8) {
        parcours = lire_bit_arbre(arbre_huffman, parcours, buffer_c[j]);
        j++;
    }

    /* si il y a encore des octets à lire */

    if (fscanf(f, "%c", &c_next) == 1) loop = 1; else loop = 0;
    c = c_next;
    while (loop) {
        if (fscanf(f, "%c", &c_next) == 1) loop = 1; else break;
        
        char2bin(c, buffer_r);

        for (i_r = 0; i_r < 8; i_r++) {
            parcours = lire_bit_arbre(arbre_huffman, parcours, buffer_r[i_r]);
        }

        c = c_next;
    }

    /* on est sur le dernier octet du fichier: il faut terminer la lecture sans le dépassement */
    char2bin(c, buffer_r);
    for (i_r = 0; i_r < 8 - depassement; i_r++) {
        parcours = lire_bit_arbre(arbre_huffman, parcours, buffer_r[i_r]);
    }

    return arbre_huffman;
}


void inserer_arbre_aux(noeud * a, char c, char * code, int pos) {
    if (code[pos] == '\0') {
        /* si on est à la fin */
        a->c = c;
    }
    else {
        
        if (code[pos] == '0') { /* on doit aller à gauche */
            if (a->gauche == NULL) {
                a->gauche = creer_noeud('.', NULL, NULL);
            }
            inserer_arbre_aux(a->gauche, c, code, pos + 1);
        }
        
        if (code[pos] == '1') { /* on doit aller à gauche */
            if (a->droit == NULL) {
                a->droit = creer_noeud('.', NULL, NULL);
            }
            inserer_arbre_aux(a->droit, c, code, pos + 1);
        }
    }
}

/* insère une feuille c dans l'arbre de huffman avec son codage */
void inserer_arbre(noeud * arbre_huffman, noeud * a) {
    char * chaine_code;
    
    /* générer la chaîne de caractères */
    /* nécessaire de passer par une chaîne de caractères pour lire "à l'envers" */
    chaine_code = int2string(a->codage, a->nb_bits);

    printf("%s\n", chaine_code);

    inserer_arbre_aux(arbre_huffman, a->c, chaine_code, 0);
}


noeud * creer_huffman_inverse(noeud * alphabet[256]) {
    noeud * arbre_huffman;
    int i;
    
    printf("DEBUT CREER HUFFMAN INVERSE\n");

    arbre_huffman = creer_noeud('.', NULL, NULL);

    for (i = 0; i < 256; i++) {
        if (alphabet[i] != NULL) {
            printf("%c %d %d\n", alphabet[i]->c, alphabet[i]->codage, alphabet[i]->nb_bits);
            
            inserer_arbre(arbre_huffman, alphabet[i]);
        }
    }

    return arbre_huffman;
}



void decompresser_fichier(FILE * f, noeud * arbre_huffman) {
    noeud * parcours;

    char c; /* utilisé pour lire 8 bits dans le fichier compressé */
    int buffer_c[8]; /* "pelleteuse": c mais en tableau de int */
    int j;

    while (fscanf(f, "%c", &c) == 1) {
        char2bin(c, buffer_c);

        printf("%c");
    }
}




int main() {
    FILE * f;

    noeud * arbre_huffman;

    int i;
    
    /*
    char c = 'E';

    int i = 0;

    char2bin(c, b_c);

    printf("%c\n", c);
    for (i = 0; i < 8; i++) {
        printf("%d", b_c[i]);
        }*/

    /* ouverture du fichier */
    f = fopen("test_fichier.txt.comphuff", "r");
    if (f == NULL) printf("fuck");

    printf("\n");
    arbre_huffman = boucle_decomp(f);

    afficher_arbre_graphique(arbre_huffman);

    /* USELESS */
    /* decompresser_fichier(f, arbre_huffman); */

    exit(EXIT_SUCCESS);
}
