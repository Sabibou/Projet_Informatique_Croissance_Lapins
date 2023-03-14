/* 
  
*/

#include <stdio.h>
#include <math.h>
#include <stdlib.h>
#include "mersenne_twister.h"

float POURCENT_LITTER[5] = {0.07, 0.27, 0.32, 0.29, 0.05};

typedef struct population{

    int* array;
    int index;
    int size;
    int nb_rabbit;

}population;

long int rabbit_population_simulation(int nb_months){

    //le premier mois est le nb_couple_this_month
    //et on a un couple jeune
    long int nb_couples_this_month = 1; 
    long int nb_couples_last_month = 0;
    long int to_keep;

    //au commence au deuxieme mois
    for(int i=2; i<=nb_months; i++){

        to_keep = nb_couples_this_month;
        nb_couples_this_month = nb_couples_last_month + nb_couples_this_month;
        nb_couples_last_month = to_keep;
        printf("%ld\n", nb_couples_this_month);
    }

    return nb_couples_this_month;
}



population* create_new_population(){

    population* p = malloc(sizeof(population)*1);
    p->array = malloc(sizeof(int) * 1000);

    for(int i=0; i<1000; i++){

        p->array[i] = 0;
    }

    p->index = 0;
    p->size = 1000;
    p->nb_rabbit = 0;

    return p;
}


void double_array(population* p){

    int* new_array = malloc(sizeof(int) * (p->size * 2));
    //on copie le contenu
    int index = 0;
    int start_copy = 0;
    int final = p->index;
    p->nb_rabbit = 0;
    while(index < final){

        if(p->array[index]==0){

            start_copy++;
            final--;
            while(p->array[start_copy]==0){ 

                start_copy++;
                final--;
            }
        }

        new_array[index] = p->array[start_copy];
        index++;

    }
    for(int i=0; i<index; i++){


        new_array[i] = p->array[i];
    }
    free(p->array);
    p->array = new_array;
    p->size = p->size * 2;
    p->index = index;

}

/*
un lapin est désigné par un entier :
 - supérieur à 200 le lapin est une femelle
 - supérieur à 100 le lapin est un mâle
*/
int create_new_rabbit(){

    float random = genrand_real1();

    if(random > 0.5){

        return 200;
    } 
    else{

        return 100;
    }

}

int nb_litter_per_year(){

    double draw;
    int j;

    double * cumulative_prob = malloc(sizeof(double) * 5); //tableau de densité cumulative
    int * nb_per_litter = malloc(sizeof(double) * 5);  //tableau de la population de chaque classe

    for(int i=0; i<5; i++){

        nb_per_litter[i] = 0;  //on initialise à 0 la population de chaque classe
    }

    cumulative_prob[0] = POURCENT_LITTER[0];

    for(int i=1; i<5; i++){

        cumulative_prob[i] = cumulative_prob[i-1] + POURCENT_LITTER[i];
    }

    for(int i=0; i<1000; i++){

        draw = genrand_real1();
        j=0;

        while(j < 5){
             
            if(draw <= cumulative_prob[j]){  //si le nombre tiré est inférieur ou égale à la densité cumulative de la classe j
                nb_per_litter[j]++;      //on incrémente la population de la classe j
                break;
            }
            
            j++;
        }
    }

    int index_max = 0;
    for(int i=0; i<5; i++){

        if(nb_per_litter[i] > nb_per_litter[index_max]){

            index_max = i;
        }
    }

    free(nb_per_litter);
    free(cumulative_prob);

    return index_max + 4;
}

void gave_birth(population* p){

    int nb_litter = nb_litter_per_year();  
    

    for(int i=0; i<nb_litter; i++){

        int rabbits_per_litter = genrand_int32() % 4 + 3;  
        //printf("%d\n", rabbits_per_litter);

        if((p->size - p->index) <= rabbits_per_litter){

            double_array(p);
        }
        int index = p->index;
        for(int i=index; i<(rabbits_per_litter + index);i++){

            p->array[i] = create_new_rabbit();
            p->index++;
            p->nb_rabbit++;
        }
    }
}

void death(population* p, int index){

    int age = p->array[index]%100;
    float survival_rate = genrand_real1();

    if(age > 0){

        if((survival_rate + ((float)(age%10)/10)) > 0.6){

            p->array[index] = 0; //meurt
            p->nb_rabbit--;
        }
    }
    /*
    else if(p->population[index]%100 > 0){

        if(genrand_real1() > 0.6){

            p->population[index] = 0; //meurt
        }
    }
    */
    else{

        if(survival_rate > 0.35){

            p->array[index] = 0; //meurt
            p->nb_rabbit--;
        }
    }
}

void get_older(population* p, int index){

    p->array[index]++;
}

void life(population* p, int years){

    int current_year = 0;

    while(current_year < years && p->nb_rabbit > 0){
        int index = p->index;
        for(int i=0; i<index; i++){  //on verifie que les lapins qui taient nes avant cette année

            if(p->array[i] != 0){  //si le lapin est encore en vie

                death(p, i);
                //printf("death\n");

                if((p->array[i] - 100 > 99) && p->array[i]%100 >= 0){  //si le lapin est une femelle et adulte

                    gave_birth(p);
                    //printf("naissance\n");
                
                }

                get_older(p, i);
                //printf("get_older\n");
            }
        }
        current_year++;
        
    } 
}

int main(void)
{    
    unsigned long init[4]={0x123, 0x234, 0x345, 0x456}, length=4;
    init_by_array(init, length); 

    //printf("%ld\n", rabbit_population_simulation(30));

    population* p1 = create_new_population();

    p1->array[0] = 100;
    p1->array[1] = 200;
    p1->array[2] = 100;
    p1->array[3] = 200;
    p1->array[4] = 100;
    p1->array[5] = 200;
    p1->array[6] = 100;
    p1->array[7] = 200;
    p1->index = 8;
    p1->nb_rabbit = 8;


    life(p1, 2);
    printf("nb lapins : %d\n", p1->nb_rabbit);

    return 0;
}
