#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <string.h>
#include <time.h>
#include <sys/stat.h> 


#define MAX_CHAR 1000
#define MEGA_CHAR 10000
#define CHAR_MULT 21

#define N 30

#define NOM_FIC_GEN "tmp.txt"
#define NOM_ARCH "tmp.txt.comphuff"


#define CSV_TAILLE "taille.csv"
#define CSV_CAR_DIF "car_diff.csv"
#define CSV_FREQ_DOM "freq_dom.csv"
#define CSV_FREQ_CROISS "freq_croiss.csv"
#define CSV_NB_FICH "nb_fich.csv"
#define CSV_PROF_DOSS "prof_doss.csv"
#define CSV_PROF_DOSS_PLUS "prof_doss_plus.csv"

#define T_TAILLE 14
#define T_CAR_DIF 9
#define T_FREQ_DOM 8
#define T_FREQ_CROISS 8
#define T_NB_FICH 8
#define T_PROF_DOSS 6


void gen_fich(int nb_car, int nb_car_diff, char * nom){
    FILE* fic = NULL;
    int i;
    
    if (strcmp(nom, "") != 0){    /* nom de fichier précisé */
        if ( (fic = fopen(nom, "w")) == NULL ){
            printf("Erreur ouverture / creation du fichier %s\n", nom);
            exit(EXIT_FAILURE);
        }
    }
    else {   /* pas de nom */
        if ( (fic = fopen("gen_fich.txt", "w")) == NULL ){
            printf("Erreur ouverture / creation du fichier gen_fich.txt\n");
            exit(EXIT_FAILURE);
        }
    }
    
    for (i = 0; i < nb_car; i++){
        /* écrit les caractère dans l'interval [255; 255 - nb_car_diff] */
        fputc(255 - (rand() % nb_car_diff), fic);
    }
    
    fclose(fic);
}


double somme_tab(double t[], int n){
    double somme = 0;
    int i;
    
    for (i = 0; i < n; i++){
        somme += t[i];
    }

    return somme;
}

void gen_fich_freq(int nb_car, double freq[], int n, char * nom){

    FILE* fic = NULL;
    int i, car;
    double total = somme_tab(freq, n), eff_cumule, alea;

    if (strcmp(nom, "") != 0){    /* nom de fichier précisé */
        if ( (fic = fopen(nom, "w")) == NULL ){
            printf("Erreur ouverture / creation du fichier %s\n", nom);
            exit(EXIT_FAILURE);
        }
    }
    else {   /* pas de nom */
        if ( (fic = fopen("gen_fich.txt", "w")) == NULL ){
            printf("Erreur ouverture / creation du fichier gen_fich.txt\n");
            exit(EXIT_FAILURE);
        }
    }

    for (i = 0; i < nb_car; i++){
        alea = ((double)rand() / (double)RAND_MAX) * total;

        car = 0;
        
        eff_cumule = freq[car];
        
        while (alea > eff_cumule){
            car++;
            eff_cumule += freq[car];
        }

        if (car > n - 1){   /* erreur d'arondi potentiel */
            car = n - 1;
        }
        
        fputc(255 - car, fic);
    }

    printf("Fichier généré\n");
    
    fclose(fic);
}


int taille_fich(FILE * fic){
    int c;
    int taille = 0;
    
    while ((c = fgetc(fic)) != EOF){
        taille += 1;
    }

    return taille;
}

int main(){

    FILE * csv;
    FILE * tmp;

    char cmd[MAX_CHAR] = "./huff_v1 -c ";
    char cmd_fich[MAX_CHAR];
    char cmd_doss[MEGA_CHAR] = "./huff_v4 -c ";
    char nom_mult_fich[CHAR_MULT];
    char nom_mult_doss[CHAR_MULT];
    char chem_doss[MAX_CHAR];
    char chem_fich[MAX_CHAR];
    char chem_fich_gen[MAX_CHAR];
    
    int n, indice, i, i_mult;
    int nb_car, nb_car_diff, t_fich;
    int err;
    double taux_comp, taux_comp_loc, t_taux_comp[N];
    double min, max;
    double moyenne_comp, ecart_type;

    int i_echelle;
    
    int ech_taille[T_TAILLE] = {1, 2, 5, 10, 20, 50, 100, 200, 500, 1000, 5000, 10000, 50000, 100000};
    
    int ech_puiss_2[T_CAR_DIF] = {1, 2, 4, 8, 16, 32, 64, 128, 256};
    
    double ech_freq[T_FREQ_DOM][T_FREQ_DOM] = {{93, 1, 1, 1, 1, 1, 1, 1}, {47.47, 47.47, 1, 1, 1, 1, 1, 1}, {31.66, 31.66, 31.66, 1, 1, 1, 1, 1}, {24, 24, 24, 24, 1, 1, 1, 1}, {19.4, 19.4, 19.4, 19.4, 19.4, 1, 1, 1}, {16.33, 16.33, 16.33, 16.33, 16.33, 16.33, 1, 1}, {14.14, 14.14, 14.14, 14.14, 14.14, 14.14, 14.14, 1}, {12.5, 12.5, 12.5, 12.5, 12.5, 12.5, 12.5, 12.5}};

    double ech_freq_puiss_2[T_FREQ_CROISS] = {1, 2, 4, 8, 16, 32, 64, 128};

    int ech_fich[T_NB_FICH] = {1, 2, 5, 10, 20, 50, 100, 200};

    int ech_doss[T_PROF_DOSS] = {0, 1, 2, 5, 10, 20};
    
    srand(time(NULL));

    strcat(cmd, NOM_FIC_GEN);
    strcat(cmd, " ");
    strcat(cmd, NOM_FIC_GEN);

    strcat(cmd_doss, NOM_FIC_GEN);
    strcat(cmd_doss, " ");
    strcat(cmd_doss, "0/");
    
    /* gen test taille */

    if ((csv = fopen(CSV_TAILLE, "w")) == NULL){
        fprintf(stderr, "Erreur ouverture fichier %s", CSV_TAILLE);
        exit(EXIT_FAILURE);
    }
     
    for (i_echelle = 0; i_echelle < T_TAILLE; i_echelle++){

        nb_car = ech_taille[i_echelle];
        nb_car_diff = 32;

        /* remise à 0 */
        taux_comp = 0;
        indice = 0;

        ecart_type = 0;

        /* calcule d'un point de la courbe */
        for (n = 0; n < N; n++){

            /* génération du fichier */
            gen_fich(nb_car, nb_car_diff, NOM_FIC_GEN);

            /* compression du fichier */
            
            err = system(cmd);
            if (err != 0){
                fprintf(stderr, "Erreur appel cmd, bye\n");
                exit(EXIT_FAILURE);
            }
        
            /* calcul de la taille du fichier compresser */
            if ((tmp = fopen(NOM_ARCH, "r")) == NULL){
                fprintf(stderr, "Erreur ouverture fichier généré\n");
                exit(EXIT_FAILURE);
            }

            t_fich = taille_fich(tmp);

            taux_comp_loc = (nb_car / (double)t_fich);
            
            taux_comp += taux_comp_loc;
            t_taux_comp[indice] = taux_comp_loc;  /* pour le calcule de l'écart type : calcul du taux de compression*/
            indice++;

            fclose(tmp);

            /* calcul du meilleur et du pire */
            if (n == 0){
                min = taux_comp_loc;
                max = taux_comp_loc;
            }
            else {
                if (taux_comp < min){
                    min = taux_comp_loc;
                }
                if (taux_comp > max){
                    max = taux_comp_loc;
                }
            }
        }

        /* calcul de la moyenne des taux de compression (sans le min et le max) */
        moyenne_comp = (taux_comp - min - max) / (double)(N - 2);
    
        /* calcul de l'écart type des taux de compression (sans le min et le max) */
        for (i = 0; i < N; i++){
            ecart_type += pow((double)t_taux_comp[i] - moyenne_comp, 2.);
        }
        ecart_type = ecart_type - pow((double)min - moyenne_comp, 2.) - pow((double)max - moyenne_comp, 2.);
        ecart_type = sqrt(ecart_type / (N - 2));

        /* écrit dans le fichier csv : taille fichier original (x), moyenne des taux de compression (y), ecart type */
        fprintf(csv, "%d,%f,%f\n", nb_car, moyenne_comp, ecart_type);

    }
    
    fclose(csv);

    /***************************************************************************/
    /***************************************************************************/
    /***************************************************************************/

    /* gen test nb car diff */

    if ((csv = fopen(CSV_CAR_DIF, "w")) == NULL){
        printf("Erreur ouverture fichier %s", CSV_CAR_DIF);
        exit(EXIT_FAILURE);
    }
    
    for (i_echelle = 0; i_echelle < T_CAR_DIF; i_echelle++){

        nb_car = 10000;
        nb_car_diff = ech_puiss_2[i_echelle];

        /* remise à 0 */
        taux_comp = 0;
        indice = 0;

        ecart_type = 0;

        /* calcule d'un point de la courbe */
        for (n = 0; n < N; n++){

            /* génération du fichier */
            gen_fich(nb_car, nb_car_diff, NOM_FIC_GEN);
            
            /* compression du fichier */
            err = system(cmd);
            if (err != 0){
                printf("Erreur appel cmd, bye\n");
                exit(EXIT_FAILURE);
            }
        
            /* calcul de la taille du fichier compresser */
            if ((tmp = fopen(NOM_ARCH, "r")) == NULL){
                printf("Erreur ouverture fichier généré\n");
                exit(EXIT_FAILURE);
            }

            t_fich = taille_fich(tmp);

            taux_comp_loc = (nb_car / (double)t_fich);
            
            taux_comp += taux_comp_loc;
            t_taux_comp[indice] = taux_comp_loc;  /* pour le calcule de l'écart type : calcul du taux de compression*/
            indice++;

            fclose(tmp);

            /* calcul du meilleur et du pire */
            if (n == 0){
                min = taux_comp_loc;
                max = taux_comp_loc;
            }
            else {
                if (taux_comp < min){
                    min = taux_comp_loc;
                }
                if (taux_comp > max){
                    max = taux_comp_loc;
                }
            }
        }

        /* calcul de la moyenne des taux de compression (sans le min et le max) */
        moyenne_comp = (taux_comp - min - max) / (double)(N - 2);
    
        /* calcul de l'écart type des taux de compression (sans le min et le max) */
        for (i = 0; i < N; i++){
            ecart_type += pow((double)t_taux_comp[i] - moyenne_comp, 2.);
        }
        ecart_type = ecart_type - pow((double)min - moyenne_comp, 2.) - pow((double)max - moyenne_comp, 2.);
        ecart_type = sqrt(ecart_type / (N - 2));

        /* écrit dans le fichier csv : nombre de caractère différent (x), moyenne des taux de compression (y), ecart type */
        fprintf(csv, "%d,%f,%f\n", nb_car_diff, moyenne_comp, ecart_type);

    }
    
    fclose(csv);

    /***************************************************************************/
    /***************************************************************************/
    /***************************************************************************/

    /* gen test freq carac dominant */

    if ((csv = fopen(CSV_FREQ_DOM, "w")) == NULL){
        printf("Erreur ouverture fichier %s", CSV_FREQ_DOM);
        exit(EXIT_FAILURE);
    }
            
    for (i_echelle = 0; i_echelle < T_FREQ_DOM; i_echelle++){

        nb_car = 10000;
        nb_car_diff = 8;

        /* remise à 0 */
        taux_comp = 0;
        indice = 0;

        ecart_type = 0;
   
        /* calcule d'un point de la courbe */
        for (n = 0; n < N; n++){

            /* génération du fichier */
            gen_fich_freq(nb_car, ech_freq[i_echelle], T_FREQ_DOM, NOM_FIC_GEN);
            
            /* compression du fichier */
            err = system(cmd);
            if (err != 0){
                printf("Erreur appel cmd, bye\n");
                exit(EXIT_FAILURE);
            }
        
            /* calcul de la taille du fichier compresser */
            if ((tmp = fopen(NOM_ARCH, "r")) == NULL){
                printf("Erreur ouverture fichier généré\n");
                exit(EXIT_FAILURE);
            }

            t_fich = taille_fich(tmp);
            
            taux_comp_loc = (nb_car / (double)t_fich);
            
            taux_comp += taux_comp_loc;
            t_taux_comp[indice] = taux_comp_loc;  /* pour le calcule de l'écart type : calcul du taux de compression*/
            
            indice++;

            fclose(tmp);

            /* calcul du meilleur et du pire */
            if (n == 0){
                min = taux_comp_loc;
                max = taux_comp_loc;
            }
            else {
                if (taux_comp < min){
                    min = taux_comp_loc;
                }
                if (taux_comp > max){
                    max = taux_comp_loc;
                }
            }
        }

        /* calcul de la moyenne des taux de compression (sans le min et le max) */
        moyenne_comp = (taux_comp - min - max) / (double)(N - 2);
    
        /* calcul de l'écart type des taux de compression (sans le min et le max) */
        for (i = 0; i < N; i++){
            ecart_type += pow((double)t_taux_comp[i] - moyenne_comp, 2.);
        }
        ecart_type = ecart_type - pow((double)min - moyenne_comp, 2.) - pow((double)max - moyenne_comp, 2.);
        ecart_type = sqrt(ecart_type / (N - 2));

        /* écrit dans le fichier csv : nombre de caractère dominant (x), moyenne des taux de compression (y), ecart type */
        fprintf(csv, "%d,%f,%f\n", i_echelle + 1, moyenne_comp, ecart_type);

    }
    
    fclose(csv);

    /***************************************************************************/
    /***************************************************************************/
    /***************************************************************************/

    /* test freq croissante */

    if ((csv = fopen(CSV_FREQ_CROISS, "w")) == NULL){
        printf("Erreur ouverture fichier %s", CSV_FREQ_CROISS);
        exit(EXIT_FAILURE);
    }
            
    for (i_echelle = 0; i_echelle < T_TAILLE; i_echelle++){
        
        nb_car = ech_taille[i_echelle];
        nb_car_diff = 8;

        /* remise à 0 */
        taux_comp = 0;
        indice = 0;

        ecart_type = 0;

        /* calcule d'un point de la courbe */
        for (n = 0; n < N; n++){

            /* génération du fichier */
            gen_fich_freq(nb_car, ech_freq_puiss_2, T_FREQ_CROISS, NOM_FIC_GEN);
            
            /* compression du fichier */
            err = system(cmd);
            if (err != 0){
                printf("Erreur appel cmd, bye\n");
                exit(EXIT_FAILURE);
            }
        
            /* calcul de la taille du fichier compresser */
            if ((tmp = fopen(NOM_ARCH, "r")) == NULL){
                printf("Erreur ouverture fichier généré\n");
                exit(EXIT_FAILURE);
            }

            t_fich = taille_fich(tmp);
            
            taux_comp_loc = (nb_car / (double)t_fich);
            
            taux_comp += taux_comp_loc;
            t_taux_comp[indice] = taux_comp_loc;  /* pour le calcule de l'écart type : calcul du taux de compression*/
            
            indice++;

            fclose(tmp);

            /* calcul du meilleur et du pire */
            if (n == 0){
                min = taux_comp_loc;
                max = taux_comp_loc;
            }
            else {
                if (taux_comp < min){
                    min = taux_comp_loc;
                }
                if (taux_comp > max){
                    max = taux_comp_loc;
                }
            }
        }

        /* calcul de la moyenne des taux de compression (sans le min et le max) */
        moyenne_comp = (taux_comp - min - max) / (double)(N - 2);
    
        /* calcul de l'écart type des taux de compression (sans le min et le max) */
        for (i = 0; i < N; i++){
            ecart_type += pow((double)t_taux_comp[i] - moyenne_comp, 2.);
        }
        ecart_type = ecart_type - pow((double)min - moyenne_comp, 2.) - pow((double)max - moyenne_comp, 2.);
        ecart_type = sqrt(ecart_type / (N - 2));

        /* écrit dans le fichier csv : taille fichier original (x), moyenne des taux de compression (y), ecart type */
        fprintf(csv, "%d,%f,%f\n", nb_car, moyenne_comp, ecart_type);

    }
    
    fclose(csv);

    /***************************************************************************/
    /***************************************************************************/
    /***************************************************************************/

    /* test nb fichier */
    
    if ((csv = fopen(CSV_NB_FICH, "w")) == NULL){
        printf("Erreur ouverture fichier %s", CSV_NB_FICH);
        exit(EXIT_FAILURE);
    }

    for (i_echelle = 0; i_echelle < T_NB_FICH; i_echelle++){
        
        nb_car = 1000;
        nb_car_diff = 64;

        /* remise à 0 */
        taux_comp = 0;
        indice = 0;

        ecart_type = 0;

        /* calcule d'un point de la courbe */
        for (n = 0; n < N; n++){

            cmd_fich[0] = '\0';
            strcat(cmd_fich, "./huff_v2 -c ");
            strcat(cmd_fich, NOM_FIC_GEN);
            
            /* génération des fichiers */
            for (i_mult = 0; i_mult < ech_fich[i_echelle]; i_mult++){
                sprintf(nom_mult_fich, "%d", i_mult);
                gen_fich(nb_car, nb_car_diff, nom_mult_fich);
                strcat(cmd_fich, " ");
                strcat(cmd_fich, nom_mult_fich);
            }
            
            /* compression du fichier */
            err = system(cmd_fich);
            if (err != 0){
                printf("Erreur appel cmd, bye\n");
                exit(EXIT_FAILURE);
            }
        
            /* calcul de la taille du fichier compresser */
            if ((tmp = fopen(NOM_ARCH, "r")) == NULL){
                printf("Erreur ouverture fichier généré\n");
                exit(EXIT_FAILURE);
            }

            t_fich = taille_fich(tmp);
            
            taux_comp_loc = ((nb_car * ech_fich[i_echelle]) / (double)t_fich);
            
            taux_comp += taux_comp_loc;
            t_taux_comp[indice] = taux_comp_loc;  /* pour le calcule de l'écart type : calcul du taux de compression*/
            
            indice++;

            fclose(tmp);

            /* calcul du meilleur et du pire */
            if (n == 0){
                min = taux_comp_loc;
                max = taux_comp_loc;
            }
            else {
                if (taux_comp < min){
                    min = taux_comp_loc;
                }
                if (taux_comp > max){
                    max = taux_comp_loc;
                }
            }
        }

        /* calcul de la moyenne des taux de compression (sans le min et le max) */
        moyenne_comp = (taux_comp - min - max) / (double)(N - 2);
    
        /* calcul de l'écart type des taux de compression (sans le min et le max) */
        for (i = 0; i < N; i++){
            ecart_type += pow((double)t_taux_comp[i] - moyenne_comp, 2.);
        }
        ecart_type = ecart_type - pow((double)min - moyenne_comp, 2.) - pow((double)max - moyenne_comp, 2.);
        ecart_type = sqrt(ecart_type / (N - 2));

        /* écrit dans le fichier csv : nombre de fichier (x), moyenne des taux de compression (y), ecart type */
        fprintf(csv, "%d,%f,%f\n", ech_fich[i_echelle], moyenne_comp, ecart_type);

    }
    
    fclose(csv);

    /* suppression des fichiers */
    for (i_mult = 0; i_mult < ech_fich[T_NB_FICH - 1]; i_mult++){
        sprintf(nom_mult_fich, "%d", i_mult);
        remove(nom_mult_fich);
    }

    /***************************************************************************/
    /***************************************************************************/
    /***************************************************************************/


    /*test prof dossier */
    if ((csv = fopen(CSV_PROF_DOSS, "w")) == NULL){
        printf("Erreur ouverture fichier %s", CSV_PROF_DOSS);
        exit(EXIT_FAILURE);
    }
    
    for (i_echelle = 0; i_echelle < T_PROF_DOSS; i_echelle++){
       
        nb_car = 1000;
        nb_car_diff = 32;

        /* remise à 0 */
        taux_comp = 0;
        indice = 0;

        ecart_type = 0;

        if (i_echelle == 0){
            sprintf(nom_mult_doss, "%d", 0);

            strcat(chem_doss, nom_mult_doss);
            strcat(chem_doss, "/");
                
            err = mkdir(chem_doss, 0777);
            if (err != 0){
                fprintf(stderr, "Le dossier %s existe déjà\n", chem_doss);
            }
        }
        else {
            for (i = ech_doss[i_echelle - 1] + 1; i <= ech_doss[i_echelle]; i++){
                sprintf(nom_mult_doss, "%d", i);

                strcat(chem_doss, nom_mult_doss);
                strcat(chem_doss, "/");
                
                err = mkdir(chem_doss, 0777);
                if (err != 0){
                    fprintf(stderr, "Le dossier %s existe déjà\n", chem_doss);
                }
            }
        }

        /* calcule d'un point de la courbe */
        for (n = 0; n < N; n++){

            chem_fich[0] = '\0';
            
            /* génération des fichiers */
            for (i_mult = 0; i_mult <= ech_doss[i_echelle]; i_mult++){

                sprintf(nom_mult_doss, "%d", i_mult);
                
                strcat(chem_fich, nom_mult_doss);
                strcat(chem_fich, "/");

                chem_fich_gen[0] = '\0';
                strcat(chem_fich_gen, chem_fich);
                strcat(chem_fich_gen, "0");
            
                gen_fich(nb_car, nb_car_diff, chem_fich_gen);

            }
          
            /* compression du fichier */
            err = system(cmd_doss);
            if (err != 0){
                printf("Erreur appel cmd, bye\n");
                exit(EXIT_FAILURE);
            }
        
            /* calcul de la taille du fichier compresser */
            if ((tmp = fopen(NOM_ARCH, "r")) == NULL){
                printf("Erreur ouverture fichier généré\n");
                exit(EXIT_FAILURE);
            }

            t_fich = taille_fich(tmp);
            
            taux_comp_loc = (nb_car * (ech_doss[i_echelle] + 1) / (double)t_fich);
            
            taux_comp += taux_comp_loc;
            t_taux_comp[indice] = taux_comp_loc;  /* pour le calcule de l'écart type : calcul du taux de compression*/

            indice++;

            fclose(tmp);

            /* calcul du meilleur et du pire */
            if (n == 0){
                min = taux_comp_loc;
                max = taux_comp_loc;
            }
            else {
                if (taux_comp < min){
                    min = taux_comp_loc;
                }
                if (taux_comp > max){
                    max = taux_comp_loc;
                }
            }
        }

        /* calcul de la moyenne des taux de compression (sans le min et le max) */
        moyenne_comp = (taux_comp - min - max) / (double)(N - 2);
    
        /* calcul de l'écart type des taux de compression (sans le min et le max) */
        for (i = 0; i < N; i++){
            ecart_type += pow((double)t_taux_comp[i] - moyenne_comp, 2.);
        }
        ecart_type = ecart_type - pow((double)min - moyenne_comp, 2.) - pow((double)max - moyenne_comp, 2.);
        ecart_type = sqrt(ecart_type / (N - 2));

        /* écrit dans le fichier csv : nombre de fichier (x), moyenne des taux de compression (y), ecart type */
        fprintf(csv, "%d,%f,%f\n", ech_doss[i_echelle], moyenne_comp, ecart_type);

    }
    
    fclose(csv);

    /* suppression des dossiers imbriquer */
    
    err = system("rm -r 0/");
    if (err != 0){
        printf("Erreur suppression dossier 0/\n");
    }
    
    
    /***************************************************************************/
    /***************************************************************************/
    /***************************************************************************/
    
    /* suppression des fichiers temporaire */
    remove(NOM_FIC_GEN);
    remove(NOM_ARCH);


    exit(EXIT_SUCCESS);
}