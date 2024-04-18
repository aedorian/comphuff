#include <stdio.h>
#include <stdlib.h>


void usage(char *s){
    printf("Usage %s : <entier1> <entier2> <entier3> [<string>]\n", s);
    printf("<entier1> : nombre de caractère >= 0\n");
    printf("<entier2> : nombre de caractère différent compris entre 1 et 256\n");
    printf("<entier3> : graine de génération >= 0\n");
    printf("<string> : (facultatif) nom du fichier\n");
}


/* nombre carac / nombre carac diff / graine / (nom) */
int main(int argc, char *argv[]){

    FILE * fic = NULL;
    int nb_car, nb_car_diff, graine;
    int i;
    
    if (argc < 4 || (nb_car = atoi(argv[1])) < 0 || (nb_car_diff = atoi(argv[2])) <= 0 || nb_car_diff > 256 || (graine = atoi(argv[3])) < 0){
        usage(argv[0]);
        exit(EXIT_FAILURE);
    }

    if (argc > 4){
        if ( (fic = fopen(argv[4], "w")) == NULL ){
            printf("Erreur ouverture / creation du fichier %s\n", argv[3]);
            exit(EXIT_FAILURE);
        }
    }
    else {
        if ( (fic = fopen("gen_fich.txt", "w")) == NULL ){
            printf("Erreur ouverture / creation du fichier gen_fich.txt\n");
            exit(EXIT_FAILURE);
        }
    }

    srand(graine);
    
    for (i = 0; i < nb_car; i++){
        fputc(rand() % nb_car_diff + 'a', fic);
    }

    printf("Fichier généré\n");
    
    fclose(fic);
    
    exit(EXIT_SUCCESS);
}