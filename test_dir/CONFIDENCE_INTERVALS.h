#include <stdio.h>
#ifndef CONFIDENCE_INTERVALS
#define CONFIDENCE_INTERVALS

double mean(int * tab, int n);
typedef struct confidenceIntervals
{
    double lowerbound; //borne inférieure
    double upperbound; //borne supérieure

} confInt;
confInt confidenceIntervals(int * tab, int nrep, double t);


#endif
