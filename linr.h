#ifndef LINR_H_INCLUDED
#define LINR_H_INCLUDED

#include <stdbool.h>

typedef struct Data {
    double* x;
    double* y;
    int N;
    int counter;
} Data, *pData;

typedef struct Result {
    double sumX;
    double sumY;
    double sumXY;
    double sumX2;
    double sumY2;
    double m;       // coeffitient
    double b;       // constant
    int N;
} Result, *pResult;

bool calcCoeffitient(Result*);
bool calcConstantTerm(Result*);
bool linRInit(Data* d, Result*);
bool linRPushData(Data*, Result*, double, double);
double linRPredict(Result* r, double x);
void linRShow (Result*);
void linRCalc(Result*);
double linRErrorSquare(Data*, Result*);
double linRError(Data*, Result*, double);

#endif
