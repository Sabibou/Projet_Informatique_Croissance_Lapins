#include <stdio.h>
#ifndef CONFIDENCE_INTERVALS
#define CONFIDENCE_INTERVALS

double mean(int * tab, int n);
void errors(double mean);
confInt confidenceIntervals(int * tab, int nrep, double t);


#endif
