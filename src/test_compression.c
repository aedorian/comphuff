#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include "gen_fich.h"

#define N 20

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
    int n, indice, i;
    int nb_car, nb_car_diff, graine;
    int taille_comp, t_taille_comp[N];
    double moyenne_comp, ecart_type;


    nb_car = 50;
    nb_car_diff = 20;
    graine = 0;

    taille_comp = 0;
    indice = 0;

    ecart_type = 0;
   
    if ((csv = fopen("analyse.csv", "w")) == NULL){
        printf("Erreur ouverture fichier analyse.csv");
        exit(EXIT_FAILURE);
    }

    /* calcule d'un point de la courbe */
    for (n = 0; n < N; n++){

        /* génération du fichier */
        gen_fich(nb_car, nb_car_diff, graine, "tmp.txt");
        graine++;

        /* compression du fichier */
        
        /* calcul de la taille du fichier compresser */
        if ((tmp = fopen("tmp.txt.comphuff", "r")) == NULL){
            printf("Erreur ouverture fichier généré\n");
            exit(EXIT_FAILURE);
        }
        
        taille_comp += taille_fich(tmp);
        t_taille_comp[indice] = taille_comp;  /* pour le calcule de l'écart type */
        indice++;

        fclose(tmp);
    }

    /* calcul de la moyenne des tailles de fichier compresser */
    moyenne = taille_comp / (double)N;
    
    /* calcul de l'écart type des tailles de fichier compresser */
    for (i = 0; i < N; i++){
        ecart_type += (t_taille_comp[i] - moyenne) * (t_taille_comp[i] - moyenne);
    }
    ecart_type = sqrt(ecart_type / (double)N);

    /* écrit dans le fichier csv : taille fichier original pour la première courbe, moyenne des tailles de fichier compresser et l'écart type de celui ci */
    fprintf(csv, "%d,%lf,%lf", nb_car, moyenne, ecart_type);
    
    fclose(csv);
    
    exit(EXIT_SUCCESS);
}