#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <math.h>

#include "linr.h"

bool calcCoeffitient(Result* r) {
    double a = r->N * r->sumXY - r->sumX * r->sumY;
    double b = r->N * r->sumX2 - pow(r->sumX, 2);
    r->m = a / b;
    return true;
}

bool calcConstantTerm(Result *r) {
    double a = r->sumY * r->sumX2 - r->sumX * r->sumXY;
    double b = r->N * r->sumX2 - pow(r->sumX, 2);
    r->b = a / b;
    return true;
}

bool linRInit(Data* d, Result* r) {
    if (d->N < 1)
        return false;
    d->x = (double*)calloc(d->N, sizeof(double));
    d->y = (double*)calloc(d->N, sizeof(double));
    if (!d->x || !d->y)
        return false;
    d->counter = 0;
    r->m = 0;
    r->b = 0;
    r->sumX = 0;
    r->sumY = 0;
    r->sumXY = 0;
    r->sumX2 = 0;
    r->sumY2 = 0;
    r->N = d->N;
    return true;
}

bool linRPushData(Data *d, Result* r, double x, double y) {
    r->sumXY += x*y;
    r->sumX += x;
    r->sumY += y;
    r->sumX2 += pow(x, 2);
    r->sumY2 += pow(y, 2);
    d->x[d->counter] = x;
    d->y[d->counter] = y;
    d->counter++;
    return true;
}

double linRPredict(Result* r, double x) {
    return r->m * x + r->b;
}

void linRShow(Result* r) {
    printf ("y = %lfx + %lf\n", r->m, r->b);
}

void linRCalc(Result *r) {
    if (r->m == 0.0 && r->b == 0.0)
        calcCoeffitient(r),
        calcConstantTerm(r);
}

double linRErrorSquare(Data *d, Result *r) {
    double temp = 0.0;
    int i = -1;
    while (++i < d->N)
        temp += pow(linRPredict(r, d->x[i]) - d->y[i],2);
    return temp;
}

double linRError(Data* d, Result* r, double x) {
    int i = -1;
    while (++i < d->N)
        if (x == d->x[i])
            return d->y[i] - linRPredict(r, d->x[i]);
    return 0.0;
}
