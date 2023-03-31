/* ---------------------------------------------------------------------------------------------  */
/* Lab4.c       Codage d'un programme simlant la croissance de lapins suivant des lois            */
/*              de probabilités qu'on définit et qui se rapprochent de la réalité                 */
/*                                                                                                */
/* ---------------------------------------------------------------------------------------------- */


#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include "MERSENNE_TWISTER.h"

float POURCENT_LITTER[5] = {0.07, 0.27, 0.32, 0.29, 0.05};
float POURCENT_MATURITY[4] = {0.15, 0.27, 0.47, 0.11};

/*--------------------------------------------------------------------------------------------- */
/* struct population    Permet la mise en place de la structure de la  population.              */
/*                      Elle permet de créer une liste doublement chaînée de structures rabbit. */
/*                      Elle permet aussi de répertorier l'effectif de la population.           */
/*                                                                                              */
/*----------------------------------------------------------------------------------------------*/

typedef struct population
{

    struct rabbit * start;
    struct rabbit * end;
    int nb_rabbit; // nombre de lapins vivants
    int nb_total_rabbit; // nombre total de lapins

}population;

/*-------------------------------------------------------------------------------------------*/
/* struct rabbit    Permet regroupe l'ensemble des attributs permettant                      */
/*                  d'identifier un lapin (sexe, âge, ...). Il possède aussi des pointeurs   */
/*                  vers la structure rabbit précédante et suivante dans la liste            */
/*                                                                                           */
/*-------------------------------------------------------------------------------------------*/

typedef struct rabbit
{

    struct rabbit * next;
    struct rabbit * previous;
    int sex; //1 male, 0 femelle
    int age;
    int nb_litter;
    int max_nb_litter;
    int maturity;

}rabbit;

/*---------------------------------------------------------------------------------------*/
/* rabbitPopulationSimulation    Implémente la suite de fibonnacci de façcon récursive   */
/*                               pour le cas de l'évolution d'une population de lapins   */
/*                                                                                       */
/*  En entrée:                   nb_months un entier                                     */
/*                                                                                       */
/*  En sortie:                   Retourne la valeur de fib(nb_months)                    */
/*---------------------------------------------------------------------------------------*/

long int rabbitPopulationSimulation(int nb_months)
{

    if(nb_months == 0)
    {

        return 0;
    }
    else if(nb_months == 1)
    {

        return 1;
    }
    else
    {

        return (rabbitPopulationSimulation(nb_months - 1) + rabbitPopulationSimulation(nb_months - 2));
    }
} 

/*-------------------------------------------------------------------------------------*/ 
/* createNewPopulation    Permet la création et initialisation d'une                   */
/*                        structure population.                                        */
/*                                                                                     */
/* En entrée:             Rien                                                         */
/*                                                                                     */
/* En sortie:             Retourne un pointeur sur la structure population créée avec  */
/*                        ses valeurs initialisées                                     */
/*-------------------------------------------------------------------------------------*/

population * createNewPopulation()
{
    
    population * p = malloc(sizeof(population)*1);
    p->start = NULL;
    p->end = NULL;
    p->nb_rabbit = 0;
    p->nb_total_rabbit = 0;
    return p;
}

/*-------------------------------------------------------------------------------*/ 
/* freePopulation    Libère la mémoire alloué par l'ensemble de la liste         */
/*                   (de toutes les structures lapins, ainsi que de la structure */
/*                   population)                                                 */
/*                                                                               */
/* En entrée:        p un pointeur sur une structure population                  */
/*                                                                               */
/* En sortie:        Rien                                                        */
/*-------------------------------------------------------------------------------*/

void freePopulation(population * p)
{

    rabbit * current_r = p->start;
    free(p);

    while(current_r->next != NULL)
    {

        current_r = current_r->next;
        free(current_r->previous);
    }

    free(current_r);
}

/*----------------------------------------------------------------------------*/
/* sortMaturity    Permet de choisir aléatoirement le mois à partir           */
/*                 duquel un lapin sera mature                                */
/*                                                                            */
/* En entrée:      Rien                                                       */
/*                                                                            */
/* En sortie:      Renvoie un entier compris entre 5 et 8 tiré selon la loi   */
/*                 de probabilité donnée par le tableau POURCENT_MATURITY     */
/*----------------------------------------------------------------------------*/

int sortMaturity()
{

    double draw;
    int j;

    double * cumulative_prob = malloc(sizeof(double) * 4); //tableau de densité cumulative
    int * month_maturity = malloc(sizeof(double) * 4);  //tableau de la population de chaque classe

    for(int i=0; i<5; i++)
    {

        month_maturity[i] = 0;  //on initialise à 0 la population de chaque classe
    }

    cumulative_prob[0] = POURCENT_MATURITY[0];

    for(int i=1; i<5; i++)
    {

        cumulative_prob[i] = cumulative_prob[i-1] + POURCENT_MATURITY[i];
    }

    for(int i=0; i<100; i++)
    {

        draw = genrand_real1();
        j=0;

        while(j < 5)
        {
             
            if(draw <= cumulative_prob[j])
            {  //si le nombre tiré est inférieur ou égale à la densité cumulative de la classe j
                month_maturity[j]++;      //on incrémente la population de la classe j
                break;
            }
            
            j++;
        }
    }

    int index_max = 0;
    for(int i=0; i<5; i++)
    {

        if(month_maturity[i] > month_maturity[index_max])
        {

            index_max = i;
        }
    }

    free(month_maturity);
    free(cumulative_prob);

    return index_max + 5; //la maturité est atteinte à partir du 5eme mois
}

/*--------------------------------------------------------------------------*/
/* createNewRabbit    Permet la création et initialisation d'une            */
/*                    structure rabbit.                                     */
/*                                                                          */
/* En entrée:          Rien                                                 */
/*                                                                          */
/* En sortie:          Renvoie un pointeur sur la structure rabbit créée et */
/*                     initialisée                                          */
/*--------------------------------------------------------------------------*/

rabbit * createNewRabbit()
{

    rabbit * r = malloc(sizeof(rabbit)*1);
    float random = genrand_real1();

    if(random > 0.5)
    {

        r->sex = 0;
    } 
    else
    {

        r->sex = 1;
    }
    r->age = 0;
    r->next = NULL;
    r->previous = NULL;
    r->nb_litter = 0;
    r->max_nb_litter = 0;
    r->maturity = sortMaturity();

    return r;

}

/*----------------------------------------------------------------------------*/ 
/* addRabbit    Permet l'ajout d'une structure rabbit à la fin de la          */
/*              liste doublement chaînée donnée par une structure population  */
/*                                                                            */
/* En entrée:   p un pointeur vers une structure population et r un           */
/*              pointeur vers une structure rabbit                            */
/*                                                                            */
/* En sortie:   Rien                                                          */
/*----------------------------------------------------------------------------*/

void addRabbit(population * p, rabbit * r)
{

    if(p->start == NULL) //si la liste est vide
    {  

        p->start = r;
        p->end = r;
    }
    else
    {
        
        r->previous = p->end;
        p->end->next = r;
        p->end = r;
    }
}

/*-----------------------------------------------------------------------------------*/ 
/* deleteRabbit    Permet la suppression d'une structure rabbit de la                */ 
/*                 liste mais aussi la libération de la mémoire de cette structure   */ 
/*                                                                                   */ 
/* En entrée:      p un pointeur vers une structure population et r un               */ 
/*                 pointeur vers une structure rabbit                                */ 
/*                                                                                   */ 
/* En sortie:      un pointeur vers la structure rabbit suivant celle supprimée      */ 
/*                 dans la liste (peut être null si la structure supprimée était la  */ 
/*                 dernière).                                                        */ 
/*-----------------------------------------------------------------------------------*/

rabbit * deleteRabbit(population * p, rabbit * r)
{

    rabbit * new_rabbit;

    if(r->next == NULL) //si le lapin est dernier
    {     
        p->end = r->previous;
    }

    if(r->previous != NULL)
    {

        new_rabbit = r->next;
        rabbit * temp = r->previous;
        temp->next = r->next;
        r->next->previous = temp;  
    }
    else
    {

        p->start = r->next;
        r->next->previous = NULL;
        new_rabbit = p->start;

    }

    free(r);  
    return new_rabbit;

}

/*----------------------------------------------------------------------------------------*/
/* nbLitterPerYear    Permet de choisir aléatoirement le nombre de portées maximums       */
/*                    que pourra avoir une lapine sur une période d'un an.                */
/*                    Met à jour l'attribut max_nb_litter de la structure avec un entier  */
/*                    compris entre 4 et 8 tiré aléatoirement selon la loi de probabilité */
/*                    établie par le tableau POURCENT_LITTER                              */
/*                                                                                        */
/* En entrée:         r un pointeur sur une structure rabbit                              */
/*                                                                                        */
/* En sortie:         Rien                                                                */
/*----------------------------------------------------------------------------------------*/                

void nbLitterPerYear(rabbit * r)
{

    r->nb_litter = 0;

    double draw;
    int j;

    double * cumulative_prob = malloc(sizeof(double) * 5); //tableau de densité cumulative
    int * nb_per_litter = malloc(sizeof(double) * 5);  //tableau de la population de chaque classe

    for(int i=0; i<5; i++)
    {

        nb_per_litter[i] = 0;  //on initialise à 0 la population de chaque classe
    }

    cumulative_prob[0] = POURCENT_LITTER[0];

    for(int i=1; i<5; i++)
    {

        cumulative_prob[i] = cumulative_prob[i-1] + POURCENT_LITTER[i];
    }

    for(int i=0; i<100; i++)
    {

        draw = genrand_real1();
        j=0;

        while(j < 5)
        {
             
            if(draw <= cumulative_prob[j]) //si le nombre tiré est inférieur ou égale
            {                              // à la densité cumulative de la classe j
                nb_per_litter[j]++;      //on incrémente la population de la classe j
                break;
            }
            
            j++;
        }
    }

    int index_max = 0;
    for(int i=0; i<5; i++)
    {

        if(nb_per_litter[i] > nb_per_litter[index_max])
        {

            index_max = i;
        }
    }

    free(nb_per_litter);
    free(cumulative_prob);

    r->max_nb_litter = index_max + 4;
}

/*----------------------------------------------------------------------------------------*/
/* gaveBirth    Implémente le mis à bas d'une portée de lapins. Si elle peut encore       */
/*              mettre à bas cette année alors elle a 85% de chances de le faire.         */      
/*              On ajoute alors à la liste entre 3 et 6 (tirés de façon uniforme)         */
/*              structures de lapins crées.  Met aussi à jour l'effectif de la population */
/*                                                                                        */
/* En entrée:   p un pointeur sur une structure population et                             */
/*              r un pointeur sur une structure rabbit                                    */
/*                                                                                        */
/* En sortie:   Rien                                                                      */
/*----------------------------------------------------------------------------------------*/

void gaveBirth(population * p, rabbit * r)
{

    if(r->nb_litter < r->max_nb_litter && genrand_real1() > 0.15)
    {

        int number_rabbits = genrand_int32() % 4 + 3;  

        for(int i=0; i<number_rabbits; i++)
        {

            addRabbit(p, createNewRabbit());
        }

        p->nb_rabbit += number_rabbits;
        p->nb_total_rabbit += number_rabbits;

    }

}

/*--------------------------------------------------------------------------*/ 
/* death      Décide de façon aléatoire si un lapin meurt durant un mois    */
/*            selon l'âge. Met à jour l'effectif de la population.          */
/*                                                                          */
/* En entrée: p un pointeur sur une structure population et                 */
/*            r un pointeur sur une structure rabbit                        */
/*                                                                          */
/* En sortie: La valeur retournée est 0 si le lapin survie et 1 s'il meurt  */
/*--------------------------------------------------------------------------*/

int death(population * p, rabbit * r)
{

    int age = r->age/12;
    float death_chance = genrand_real1();


    if(age > r->maturity)
    {
        
        float survival_rate =  pow(0.6 * pow(0.9, (age > 10) * (age%10)), 1/12);

        if(death_chance >= survival_rate * (age != 180))
        { //taux de survie mensuel de 95.8%, si le lapin a 15 ans il meurt
           
            p->nb_rabbit--;
            return 1;
        }
    }
    else
    {

        if(death_chance >= 0.91)
        {  //taux de survie mensuel de 91%

            p->nb_rabbit--;
            return 1;
        }
    }

    return 0;
}

/*---------------------------------------------------------------------*/ 
/* getOlder    Incrémente de un l'attribut âge d'une structure rabbit  */
/*                                                                     */
/* En entrée:  r un pointeur sur une structure rabbit                  */
/*                                                                     */
/* En sortie:  Rien                                                    */
/*---------------------------------------------------------------------*/

void getOlder(rabbit * r)
{

    r->age++;
}

/*--------------------------------------------------------------------------*/ 
/* life       Fonction principale simulant l'évolution d'une population de  */
/*            lapins au cours des mois.                                     */
/*            Affiche tous les mois l'effectif de la population.            */
/*                                                                          */
/* En entrée: p un pointeur sur une structure population et un entier       */
/*            months donnant le nombre de mois que l'on va simuler.         */
/*                                                                          */
/* En sortie: Rien                                                          */
/*--------------------------------------------------------------------------*/

void life(population * p, int months)
{

    int current_month = 0;

    while(current_month < months && p->nb_rabbit > 1)
    { 

        int i = 0;
        int index = p->nb_rabbit;
        rabbit* current_r = p->start;

        while(i<index && current_r->next != NULL)
        {  //on parcours que les lapins nés avant ce mois

            if(death(p, current_r))
            {  //si le lapin est mort...

                current_r = deleteRabbit(p, current_r); //...alors on le supprime et on récupère le suivant
                if(current_r == NULL)
                {   //si le suivant n'existe pas alors on passe au mois suivant

                    break;
                }
                else
                {  //sinon on passe à la boucle suivante et donc au lapin que l'on vient de récupérer

                    continue;
                }
            }

            if(current_r->age >= current_r->maturity && current_r->sex == 0)
            {  //si le lapin est une femelle et adulte
                
                if(current_r->max_nb_litter == 0 || (current_r->age - current_r->maturity)%12 == 0)
                {  //si la lapine n'a jamais été enceinte ou si ça fait un an depuis le dernier tirage de nbLitterPerYear

                    nbLitterPerYear(current_r);  //
                }

                gaveBirth(p, current_r);
            
            }

            getOlder(current_r);
            
            current_r = current_r->next; 
            i++; //on passe au lapin suivant
        }
        current_month++;  //on passe au mois suivant
        printf("%d : vivants : %d | morts : %d | total : %d\n", current_month, p->nb_rabbit, p->nb_total_rabbit-p->nb_rabbit, p->nb_total_rabbit);
    }
}

int * replicateLife(population * p, int months, int nrep)
{
    int * tab = malloc(nrep * sizeof(int));
    int nRabbit;
    for(int i=0;i<n;i++)
    {
        life(p, months);
        nRabbit = p->nb_rabbit        
        tab[i] = nRabbit;
    }
    return tab;
}


int main()
{

    unsigned long init[4]={0x123, 0x234, 0x345, 0x456}, length=4;
    init_by_array(init, length); 

    //printf("%ld\n", rabbitPopulationSimulation(30));
    int nrep = 30;
    int * tab = malloc(nrep * sizeof(int));
    int nRabbit;
    for(int i=0; i<nrep;i++)
    {

        population * p = createNewPopulation();

        rabbit * r1 = createNewRabbit();
        addRabbit(p, r1);
        r1->sex = 0;
        r1->age = 13;

        rabbit * r2 = createNewRabbit();
        addRabbit(p, r2);
        r2->sex = 0;
        r2->age = 13;

        rabbit * r4 = createNewRabbit();
        addRabbit(p, r4);
        r4->sex = 0;
        r4->age = 13;

        rabbit * r5 = createNewRabbit();
        addRabbit(p, r5);
        r5->sex = 1;
        r5->age = 13;

        rabbit * r3 = createNewRabbit();
        addRabbit(p, r3);
        r3->sex = 1;
        r3->age = 13;

        rabbit * r6 = createNewRabbit();
        addRabbit(p, r6);
        r6->sex = 1;
        r6->age = 13;

        p->nb_rabbit = 6;
        p->nb_total_rabbit = 6;

        life(p, 70);
        //printf("nb lapins : %d\n", p->nb_rabbit);
        nRabbit = p->nb_rabbit        
        tab[i] = nRabbit;
        freePopulation(p);
    }

    confInt CI = confidenceIntervals(tab,30,2.75);
    printf("The confidence intervals is [%f, %f].\n\n",CI.lowerbound, CI.upperbound);


    return 0;
}
