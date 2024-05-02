#include <stdio.h>
#include <stdlib.h>
#include <MLV/MLV_all.h>

#include <string.h>

#include "headers/arbre.h"
#include "headers/decomp.h"
#include "headers/utils.h"

/* Permet d'avancer dans l'arbre de huffman avec un pointeur parcours sur un noeud.
   S'occupe également d'écrire dans le fichier décompressé.
Retourne le noeud de parcours. */
noeud * lire_bit_arbre(noeud * arbre_huffman, noeud * parcours, int bit, FILE * fic) {

    parcours = (bit == 0) ? parcours->gauche : parcours->droit;
    
    if (est_feuille(parcours)) {
        fprintf(fic, "%c", parcours->c);
        parcours = arbre_huffman; /* retour au début */
    }


    return parcours;
}

/* Permet de décompresser le fichier binaire f formaté : dépacement, nombre de caractères différents, structure alphabet, contenu compressé.
Retourne l'abre de codage associé au fichier f compressé. */
noeud * boucle_decompresse(FILE * f, char * nom_fichier, char * chemin_dossier) {
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
    
    int char_lu = 0; /* le caractère en train d'être lu. aussi utilisé pour indexer alphabet */
    int lu; /* OBSOLETE */

    /* PARTIE ARBRE DE HUFFMAN */
    FILE * fic_decomp = NULL; /* fichier décompressé */
    char nom_decomp[100];
    
    noeud * arbre_huffman;
    noeud * parcours;
    char c_next;
    int loop;

    /* initialisation de l'alphabet */
    for (j = 0; j < 256; j++) alphabet[j] = NULL;

    /* on lit les deux premières valeurs (sur 8 bits) */
    if (fscanf(f, "%c", &depassement) != 1) {
        fprintf(stderr, "Erreur de lecture du dépacement\n");
    }
    if (fscanf(f, "%c", &nb_chars) != 1) {
        fprintf(stderr, "Erreur de lecture du nombre de caractères différents\n");
    }

    /* on lit l'alphabet entier (il y a nb_chars caractères différents) */
    nb_chars_lus = 0;
    i_r = 0;
    while (nb_chars_lus < nb_chars) {
        /* on lit 8 bits dans c */
        if (fscanf(f, "%c", &c) != 1) {
            fprintf(stderr, "Erreur de lecture de l'alphabet\n");
        }
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
                        exit(EXIT_FAILURE);  /* EXIT ??? NULL ? */
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
    /* on ne ferme pas le fichier, on l'utilisera plus tard pour lire le contenu du fichier */
    
    arbre_huffman = creer_huffman_inverse(alphabet);

    /* ECRITURE DU FICHIER */
    /* création du nom du fichier décompressé */
    if (chemin_dossier != NULL) {
        strcpy(nom_decomp, chemin_dossier);
        /* regarder si le '/' a déjà été inclus */
        strcat(nom_decomp, nom_fichier);
    }
    else {
        strcpy(nom_decomp, nom_fichier);
    }
    strcat(nom_decomp, ".decomp");
    fic_decomp = fopen(nom_decomp, "w");
    if (fic_decomp == NULL) {
        fprintf(stderr, "Erreur main: erreur lors de l'ouverture de \"%s\"\n", nom_decomp);
        exit(EXIT_FAILURE);
    }

    /* afficher_arbre_graphique(arbre_huffman); */
    
    /* on commence au début */
    parcours = arbre_huffman;
    c_next = 0; /* prochain octet, initialisé à 0 pour NULL */
    loop = 1;

    /* finir de vider le buffer restant: le dernier char lu n'a peut-être pas été lu en entier */
    while (j != 8 && j != 0) { /* j != 0 pas sûr sûr */
        parcours = lire_bit_arbre(arbre_huffman, parcours, buffer_c[j], fic_decomp);
        j++;
    }

    /* si il y a encore des octets à lire... */
    /* si on lit un octet, on boucle. sinon pas de boucle */
    if (fscanf(f, "%c", &c_next) == 1) loop = 1; else loop = 0;
    
    c = c_next; /* au début, c (le caractère analysé) est le caractère qui vient d'être lu */
    while (loop) {
        /* regarde si il y a un autre octet */
        if (fscanf(f, "%c", &c_next) != 1) {
            /* si il n'y en a pas, on arrête immédiatement la boucle */
            break;
        }
        
        char2bin(c, buffer_r);
        
        for (i_r = 0; i_r < 8; i_r++) {
            parcours = lire_bit_arbre(arbre_huffman, parcours, buffer_r[i_r], fic_decomp);
        }

        /* on sait qu'il y a eu un prochain octet: c devient ce prochain octet */
        c = c_next;
    }

    /* on est sur le dernier octet du fichier: il faut terminer la lecture sans le dépassement */
    char2bin(c, buffer_r);
    for (i_r = 0; i_r < 8 - depassement; i_r++) {
        parcours = lire_bit_arbre(arbre_huffman, parcours, buffer_r[i_r], fic_decomp);
    }

    fclose(fic_decomp);

    return arbre_huffman;
}

/* Insère une feuille c dans l'arbre de Huffman grâce à son codage. */
void inserer_arbre(noeud * a, char c, char * code, int pos) {
    if (code[pos] == '\0') {
        /* si on est à la fin: s'arrête et assigne le caractère à la feuille*/
        a->c = c;
    }
    else {
        
        if (code[pos] == '0') { /* on doit aller à gauche */
            if (a->gauche == NULL) {
                a->gauche = creer_arbre('.', NULL, NULL);
            }
            inserer_arbre(a->gauche, c, code, pos + 1);
        }
        
        if (code[pos] == '1') { /* on doit aller à gauche */
            if (a->droit == NULL) {
                a->droit = creer_arbre('.', NULL, NULL);
            }
            inserer_arbre(a->droit, c, code, pos + 1);
        }
    }
}

/* Crée l'arbre de Huffman à partir d'un alphabet,
et le retourne. */
noeud * creer_huffman_inverse(noeud * alphabet[256]) {
    noeud * arbre_huffman;
    char * chaine_code;
    int i;

    arbre_huffman = creer_arbre('.', NULL, NULL); /* crée un noeud */

    for (i = 0; i < 256; i++) {
        if (alphabet[i] != NULL) {
            
            /* générer la chaîne de caractères */
            /* nécessaire de passer par une chaîne de caractères pour lire "à l'envers" */
            chaine_code = int2string(alphabet[i]->codage, alphabet[i]->nb_bits);

            inserer_arbre(arbre_huffman, alphabet[i]->c, chaine_code, 0);
        }
    }

    return arbre_huffman;
}
