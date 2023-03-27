/* 
  
*/

#include <stdio.h>
#include <math.h>
#include <stdlib.h>
#include "mersenne_twister.h"

float POURCENT_LITTER[5] = {0.07, 0.27, 0.32, 0.29, 0.05};
float POURCENT_MATURITY[4] = {0.15, 0.27, 0.47, 0.11};

typedef struct population{

    struct rabbit* start;
    struct rabbit* end;
    int nb_rabbit;
    int nb_total_rabbit;

}population;

typedef struct rabbit{

    struct rabbit* next;
    struct rabbit* previous;
    int sex; //1 male, 0 femelle
    int age;
    int nb_litter;
    int max_nb_litter;
    int maturity;

}rabbit;


long int rabbit_population_simulation(int nb_months){

    if(nb_months == 0){

        return 0;
    }
    else if(nb_months == 1){

        return 1;
    }
    else{

        return (rabbit_population_simulation(nb_months - 1) + rabbit_population_simulation(nb_months - 2));
    }
}

population* create_new_population(){
    
    population* p = malloc(sizeof(population)*1);
    p->start = NULL;
    p->end = NULL;
    p->nb_rabbit = 0;
    p->nb_total_rabbit = 0;
    return p;
}

void free_population(population* p){

    rabbit* current_r = p->start;
    free(p);

    while(current_r->next != NULL){

        current_r = current_r->next;
        free(current_r->previous);
    }

    free(current_r);
}

int sort_maturity(){

    double draw;
    int j;

    double * cumulative_prob = malloc(sizeof(double) * 4); //tableau de densité cumulative
    int * month_maturity = malloc(sizeof(double) * 4);  //tableau de la population de chaque classe

    for(int i=0; i<5; i++){

        month_maturity[i] = 0;  //on initialise à 0 la population de chaque classe
    }

    cumulative_prob[0] = POURCENT_MATURITY[0];

    for(int i=1; i<5; i++){

        cumulative_prob[i] = cumulative_prob[i-1] + POURCENT_MATURITY[i];
    }

    for(int i=0; i<100; i++){

        draw = genrand_real1();
        j=0;

        while(j < 5){
             
            if(draw <= cumulative_prob[j]){  //si le nombre tiré est inférieur ou égale à la densité cumulative de la classe j
                month_maturity[j]++;      //on incrémente la population de la classe j
                break;
            }
            
            j++;
        }
    }

    int index_max = 0;
    for(int i=0; i<5; i++){

        if(month_maturity[i] > month_maturity[index_max]){

            index_max = i;
        }
    }

    free(month_maturity);
    free(cumulative_prob);

    return index_max + 5; //la maturité est atteinte à partir du 5eme mois
}

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
    r->nb_litter = 0;
    r->max_nb_litter = 0;
    r->maturity = sort_maturity();

    return r;

}

void add_rabbit(population* p, rabbit* r){

    if(p->start == NULL){

        p->start = r;
        p->end = r;
    }
    else{
        
        r->previous = p->end;
        p->end->next = r;
        p->end = r;
    }
}

rabbit* delete_rabbit(population* p, rabbit* r){

    rabbit* new_rabbit;

    if(r->next == NULL){     //si le lapin est dernier

        p->end = r->previous;
    }

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

void nb_litter_per_year(rabbit* r){

    r->nb_litter = 0;

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

    for(int i=0; i<100; i++){

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

    r->max_nb_litter = index_max + 4;
}

void gave_birth(population* p, rabbit* r){

    if(r->nb_litter < r->max_nb_litter && genrand_real1() > 0.15){

        int number_rabbits = genrand_int32() % 4 + 3;  

        for(int i=0; i<number_rabbits; i++){

            add_rabbit(p, create_new_rabbit());
        }

        p->nb_rabbit += number_rabbits;
        p->nb_total_rabbit += number_rabbits;

    }

}

int death(population*p, rabbit* r){

    int age = r->age/12;
    float death_chance = genrand_real1();


    if(age > r->maturity){
        
        float survival_rate =  pow(0.6 * pow(0.9, (age > 10) * (age%10)), 1/12);

        if(death_chance >= survival_rate * (age != 180)){ //taux de survie mensuel de 95.8%, si le lapin a 15 ans il meurt
           
            p->nb_rabbit--;
            return 1;
        }
    }
    else{

        if(death_chance >= 0.91){  //taux de survie mensuel de 91%

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

            if(death(p, current_r)){  //si le lapin est mort...

                current_r = delete_rabbit(p, current_r); //...alors on le supprime et on récupère le suivant
                if(current_r == NULL){   //si le suivant n'existe pas alors on passe au mois suivant

                    break;
                }
                else{  //sinon on passe au lapin suivant

                    continue;
                }
            }

            if(current_r->age >= current_r->maturity && current_r->sex == 0){  //si le lapin est une femelle et adulte
                
                if(current_r->max_nb_litter == 0 || (current_r->age - current_r->maturity)%12 == 0){   //si la lapine n'a jamais été enceinte ou si ça fait un an depuis le dernier tirage de nb_litter_per_year

                    nb_litter_per_year(current_r);
                }

                gave_birth(p, current_r);
                //printf("naissance\n");
            
            }

            get_older(current_r);
            //printf("get_older\n");
            
            current_r = current_r->next;
            i++;
        }
        current_month++;
        printf("%d : vivants : %d | morts : %d | total : %d\n", current_month, p->nb_rabbit, p->nb_total_rabbit-p->nb_rabbit, p->nb_total_rabbit);
    } 
}

int main(){

    unsigned long init[4]={0x123, 0x234, 0x345, 0x456}, length=4;
    init_by_array(init, length); 

    printf("%ld\n", rabbit_population_simulation(30));
    
    
    for(int i=0; i<5;i++){

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
            p->nb_total_rabbit = 6;
            life(p, 72);
            printf("nb lapins : %d\n", p->nb_rabbit);
            free_population(p);
    }


    return 0;
}
