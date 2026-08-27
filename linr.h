#ifndef LINR_H
#define LINR_H

#include <stdbool.h>
#include <stddef.h>

typedef struct {
    size_t n;
    double meanX;
    double meanY;
    double Sxx;
    double Sxy;
    double Syy;
    double m;
    double b;
    bool calculated;
} Result;

void linRInit(Result *r);
bool linRPush(Result *r, double x, double y);
bool linRCalc(Result *r);

double linRPredict(const Result *r, double x);
double linRError(const Result *r, double x, double y);
double linRErrorSquare(const Result *r);
double linRTotalSquare(const Result *r);
double linRR2(const Result *r);

void linRShow(const Result *r);

#endif
