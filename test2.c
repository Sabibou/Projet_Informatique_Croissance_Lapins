/* 
  
*/

#include <stdio.h>
#include <math.h>
#include <stdlib.h>
#include "mersenne_twister.h"

float POURCENT_LITTER[5] = {0.7, 0.27, 0.32, 0.29, 0.5};

typedef struct element{

    struct element* next;
    //int* population = malloc((sizeof(int)*1000);
    int population[1000];
    int nb_rabbits;
    int index_free;  //indice à partir duquel le tableau est libre

}element;

typedef struct start_list{

    element* first;

}start_list;

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

start_list* create_list(){

    start_list* start = malloc(sizeof(start_list)*1);
    start->first = NULL;

    return start;
}

element* create_new_population(){

    element* p = malloc(sizeof(element)*1);

    p->next = NULL;
    p->nb_rabbits = 0;
    p->index_free = 0;

    for(int i=0; i<1000; i++){

        p->population[i] = 0;
    } 

    return p;
}

void add_new_population(start_list* start){

    element* p = create_new_population();

    if(start->first == NULL){

        start->first = p;
    }
    else{

        element* actual_p = start->first;

        while(actual_p != NULL){

            actual_p = actual_p->next;
        }

        actual_p->next = p;
    }
    
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

    for(int i=0; i<10000; i++){

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

void gave_birth(element* p){

    int nb_litter = nb_litter_per_year();  
    int j, index_to_go;
    element* current_p = p;

    for(int i=0; i<nb_litter; i++){

        int rabbits_per_litter = genrand_int32() % 4 + 3;  

        j = current_p->index_free;
        index_to_go = rabbits_per_litter + current_p->index_free;

        while(j < index_to_go){

            if(j > 999){

                index_to_go -= current_p->index_free + j;
                current_p = new_p;
                j = current_p->index_free;
            }
            current_p->population[j] = create_new_rabbit();
            current_p->nb_rabbits++;
            current_p->index_free++;
            j++;
        }
    }
}

void death(element* p, int index){

    int age = p->population[index]%100;
    float survival_rate = genrand_real1();

    if(age > 0){

        if((survival_rate + ((float)(age%10)/10)) > 0.6){

            p->population[index] = 0; //meurt
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

            p->population[index] = 0; //meurt
        }
    }
}

void get_older(element* p, int index){

    p->population[index]++;
    p->nb_rabbits--;
}

int life(start_list* s, int years){

    int current_year = 0;
    int total_rabbits = 1;

    while(current_year < years && total_rabbits > 0){

        total_rabbits = 0;
        element* current_p = s->first;

        do{

            for(int i=0; i<current_p->index_free; i++){  //on verifie que les lapins qui taient nes avant cette année

                if(current_p->population[i] != 0){  //si le lapin est encore en vie

                    death(current_p, i);

                    if(current_p->population[i] - 100 > 100 && current_p->population[i]%100 > 0){  //si le lapin est une femelle et adulte

                        gave_birth(current_p);
                    }

                    get_older(current_p, i);
                }
            }

            total_rabbits += current_p->nb_rabbits;
            //si current_p->nb_rabbits = 0 supprimer le tableau
            current_p = current_p->next;

        }while(current_p != NULL);

    } 
    return total_rabbits;
}

int main(void)
{    
    unsigned long init[4]={0x123, 0x234, 0x345, 0x456}, length=4;
    init_by_array(init, length); 

    printf("%ld\n", rabbit_population_simulation(30));

    element* p1 = create_new_population();

    p1->population[0] = 100;
    p1->population[1] = 200;
    p1->population[2] = 100;
    p1->population[3] = 200;
    p1->population[4] = 100;
    p1->population[5] = 200;
    p1->population[6] = 100;
    p1->population[7] = 200;
    p1->index_free = 8;

    start_list* start = create_list();

    start->first = p1;

    printf("%d\n", life(start, 5));

    return 0;
}
