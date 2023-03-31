#include <stdio.h>
#include <math.h>


double mean(int * tab, int n)
{    
    double sum = 0; // initialisation de la somme des lapins à 0
    for(int i=0;i<n;i++)
    {
        sum += tab[i];
    }
    return sum / (double)n;
}

typedef struct confidenceIntervals
{
    double lowerbound; //borne inférieure
    double upperbound; //borne supérieure

} confInt;

confInt confidenceIntervals(int * tab, int nrep, double t)
{
    confInt CI;
    double var_estimate = 0;
    double sd_estimate = 0;
    double R = 0;
    double moy = mean(tab, nrep);

    for(int i =0;i<nrep;i++)
    {
        var_estimate += pow(tab[i] - moy, 2);
    }

    var_estimate = var_estimate / (nrep - 1);
    sd_estimate = sqrt(var_estimate);
    R = t * sqrt(var_estimate / nrep);
    CI.lowerbound = moy - R;
    CI.upperbound = moy + R;
    printf("\n");
    printf("L'estimateur de la variance est %f \n", var_estimate);
    printf("Lestimateur de l'écart-type est : %f \n\n", sd_estimate);

    return CI;
}