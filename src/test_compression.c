#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <string.h>
#include "gen_fich.h"

#define N 20
#define ECH 14
#define NOM_FIC_GEN "tmp.txt"  /* max 20 carac */
#define NOM_ARCH "tmp_archive" /* max 20 carac */
#define NOM_FIC_DONNEES "analyse.csv"

int taille_fich(FILE * fic){
    char c;
    int taille;
    
    while ((c = fgetc(fic)) != EOF){
        taille += 1;
    }

    return taille;
}

int main(){

    FILE * csv;
    FILE * tmp;

    char cmd[44] = "huff_vX -c tmp_archive "; /* 23 + 20 + '\0' */
    char nom_arch[30] = ""; /* 20 + ".comphuff" + '\0' */
    
    int n, indice, i;
    int nb_car, nb_car_diff, graine;
    int taille_comp, t_taille_comp[N];
    int min, max;
    double moyenne_comp, ecart_type;

    int i_echelle;
    int echelle[ECH] = {1, 2, 5, 10, 20, 50, 100, 200, 500, 1000, 5000, 10000, 50000, 100000};
        
    for (i_echelle = 0; i_echelle < ECH; i_echelle++){

        nb_car = echelle[i_echelle];
        nb_car_diff = 20;

        /* graine de génération */
        graine = 0;

        /* remise à 0 */
        taille_comp = 0;
        indice = 0;

        ecart_type = 0;
   
        if ((csv = fopen(NOM_FIC_DONNEES, "w")) == NULL){
            printf("Erreur ouverture fichier %s", NOM_FIC_DONNEES);
            exit(EXIT_FAILURE);
        }

        /* calcule d'un point de la courbe */
        for (n = 0; n < N; n++){

            /* génération du fichier */
            gen_fich(nb_car, nb_car_diff, graine, NOM_FIC_GEN);
            graine++;

            /* compression du fichier */
            strcat(cmd, NOM_FIC_GEN);
            system(cmd);
        
            /* calcul de la taille du fichier compresser */
            strcat(nom_arch, NOM_ARCH);
            strcat(nom_arch, ".comphuff");
            if ((tmp = fopen(nom_arch, "r")) == NULL){
                printf("Erreur ouverture fichier généré\n");
                exit(EXIT_FAILURE);
            }

            taille_comp += taille_fich(tmp);
            t_taille_comp[indice] = taille_comp;  /* pour le calcule de l'écart type */
            indice++;

            fclose(tmp);

            if (n == 0){
                min = taille_comp;
                max = taille_comp;
            }
            else {
                if (taille_comp < min){
                    min = taille_comp;
                }
                if (taille_comp > max){
                    max = taille_com;
                }
            }
        }

        /* calcul de la moyenne des tailles de fichier compresser (sans le min et le max) */
        moyenne_comp = (taille_comp - min - max) / (double)(N - 2);
    
        /* calcul de l'écart type des tailles de fichier compresser (sans le min et le max) */
        for (i = 0; i < N; i++){
            ecart_type += (t_taille_comp[i] - moyenne_comp) * (t_taille_comp[i] - moyenne_comp);
        }
        ecart_type = ecart_type - (min - moyenne_comp) * (min - moyenne_comp) - (max - moyenne_comp) * (max - moyenne_comp)
        ecart_type = sqrt(ecart_type / (double)(N - 2));

        /* écrit dans le fichier csv : taille fichier original pour la première courbe, moyenne des tailles de fichier compresser et l'écart type de celui ci */
        fprintf(csv, "%d,%lf,%lf\n", nb_car, moyenne_comp, ecart_type);

    }
    
    fclose(csv);
    
    exit(EXIT_SUCCESS);
}