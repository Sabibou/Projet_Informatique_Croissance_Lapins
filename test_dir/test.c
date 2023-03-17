/* 
  
*/

#include <stdio.h>
#include <math.h>
#include <stdlib.h>
#include "mersenne_twister.h"

//float POURCENT_LITTER[5] = {0.07, 0.27, 0.32, 0.29, 0.05};

typedef struct population{

    struct rabbit* start;
    int nb_rabbit;

}population;

typedef struct rabbit{

    struct rabbit* next;
    struct rabbit* previous;
    int sex; //1 male, 0 femelle
    int age;

}rabbit;


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
    p->start = NULL;
    p->nb_rabbit = 0;
    return p;
}

/*
un lapin est désigné par un entier :
 - supérieur à 200 le lapin est une femelle
 - supérieur à 100 le lapin est un mâle
*/
rabbit* create_new_rabbit(){

    rabbit* r = malloc(sizeof(rabbit)*1);
    float random = genrand_real1();

    if(random > 0.5){

        r->sex = 0;
    } 
    else{

        r->sex = 1;
    }
    r->age = 0;
    r->next = NULL;
    r->previous = NULL;

    return r;

}

void add_rabbit(population* p, rabbit* r){

    if(p->start == NULL){

        p->start = r;
    }
    else{

        rabbit* current_r = p->start;
        while(current_r->next != NULL){

            current_r = current_r->next;
        }

        current_r->next = r;
        r->previous = current_r;
    }
}

rabbit* delete_rabbit(population* p, rabbit* r){

    rabbit* new_rabbit;

    if(r->previous != NULL){

        new_rabbit = r->next;
        rabbit* temp = r->previous;
        temp->next = r->next;
        r->next->previous = temp;  
    }
    else{

        p->start = r->next;
        r->next->previous = NULL;
        new_rabbit = p->start;

    }

    free(r);  
    return new_rabbit;

}
/*
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
*/
void gave_birth(population* p){

    if(genrand_real1() > 0.5){

         int number_rabbits = genrand_int32() % 4 + 3;  
         
         for(int i=0; i<number_rabbits; i++){

             add_rabbit(p, create_new_rabbit());
             p->nb_rabbit++;
         }
    }

}

int death(population*p, rabbit* r){

    int age = r->age/12;
    float death_chance = genrand_real1();


    if(age > 6){
        
        float survival_rate =  pow(0.6 * pow(0.9, (age > 10) * ((age/12)%10)), 1/12);

        if(death_chance >= survival_rate * (age != 180)){ //taux de survie mensuel de 95.8%, si le lapin a 15 ans il meurt
            //printf("death1\n"); //meurt
            p->nb_rabbit--;
            return 1;
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

        if(death_chance >= 0.91){  //taux de survie mensuel de 91%
            //printf("death2\n");
             //meurt
            p->nb_rabbit--;
            return 1;
        }
    }

    return 0;
}

void get_older(rabbit* r){

    r->age++;
}

void life(population* p, int months){

    int current_month = 0;

    while(current_month < months && p->nb_rabbit > 0){

        int i = 0;
        int index = p->nb_rabbit;
        rabbit* current_r = p->start;

        while(i<index && current_r->next != NULL){  //on verifie que les lapins qui taient nes avant cette année

            if(death(p, current_r)){

                current_r = delete_rabbit(p, current_r);
                if(current_r == NULL){

                    break;
                }
                else{

                    continue;
                }
            }

            if(current_r->age > 6 && current_r->sex == 0){  //si le lapin est une femelle et adulte

                gave_birth(p);
                //printf("naissance\n");
            
            }

            get_older(current_r);
            //printf("get_older\n");
            
            current_r = current_r->next;
            i++;
        }
        current_month++;
        printf("%d\n", p->nb_rabbit);
    } 
}

int main(){

    unsigned long init[4]={0x123, 0x234, 0x345, 0x456}, length=4;
    init_by_array(init, length); 

    //printf("%ld\n", rabbit_population_simulation(10));
    
    population* p = create_new_population();

    rabbit* r1 = create_new_rabbit();
    add_rabbit(p, r1);
    r1->sex = 0;
    r1->age = 13;

    rabbit* r2 = create_new_rabbit();
    add_rabbit(p, r2);
    r2->sex = 0;
    r2->age = 13;

    rabbit* r4 = create_new_rabbit();
    add_rabbit(p, r4);
    r4->sex = 0;
    r4->age = 13;

    rabbit* r5 = create_new_rabbit();
    add_rabbit(p, r5);
    r5->sex = 0;
    r5->age = 13;

    rabbit* r3 = create_new_rabbit();
    add_rabbit(p, r3);
    r3->sex = 0;
    r3->age = 13;

    rabbit* r6 = create_new_rabbit();
    add_rabbit(p, r6);
    r6->sex = 0;
    r6->age = 13;

    p->nb_rabbit = 6;
    
    /*
    population p = {NULL, 0};
    rabbit r = {NULL, NULL, 1 ,0};
    rabbit ra = {NULL, NULL, 1 ,7};
    rabbit rab = {NULL, NULL, 0 ,7};
    rabbit rabi = {NULL, NULL, 1, 10};

    printf("d");

    add_rabbit(&p, &r);
    add_rabbit(&p, &ra);
    add_rabbit(&p, &rab);
    add_rabbit(&p, &rabi);
    p.nb_rabbit = 4;
    printf("%d", p.start->next->previous->age);
    */
    life(p, 100);
    printf("nb lapins : %d\n", p->nb_rabbit);

    return 0;
}
