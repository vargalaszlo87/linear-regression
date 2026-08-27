#include <stdio.h>
#include <math.h>

#include "linr.h"

#define LINR_EPSILON 1e-15

void linRInit(Result *r)
{
    if (!r)
        return;

    r->n = 0;
    r->meanX = 0.0;
    r->meanY = 0.0;
    r->Sxx = 0.0;
    r->Sxy = 0.0;
    r->Syy = 0.0;
    r->m = 0.0;
    r->b = 0.0;
    r->calculated = false;
}

bool linRPush(Result *r, double x, double y)
{
    double dx, dy;

    if (!r)
        return false;

    if (!isfinite(x) || !isfinite(y))
        return false;

    r->n++;

    dx = x - r->meanX;
    dy = y - r->meanY;

    r->meanX += dx / r->n;
    r->meanY += dy / r->n;

    r->Sxx += dx * (x - r->meanX);
    r->Sxy += dx * (y - r->meanY);
    r->Syy += dy * (y - r->meanY);

    r->calculated = false;

    return true;
}

bool linRCalc(Result *r)
{
    double tolerance;

    if (!r || r->n < 2)
        return false;

    tolerance = LINR_EPSILON * fmax(1.0, r->meanX * r->meanX * r->n);

    if (fabs(r->Sxx) <= tolerance)
        return false;

    r->m = r->Sxy / r->Sxx;
    r->b = r->meanY - r->m * r->meanX;

    if (!isfinite(r->m) || !isfinite(r->b))
        return false;

    r->calculated = true;

    return true;
}

double linRPredict(const Result *r, double x)
{
    if (!r || !r->calculated)
        return NAN;

    if (!isfinite(x))
        return NAN;

    return r->m * x + r->b;
}

double linRError(const Result *r, double x, double y)
{
    double prediction;

    if (!r)
        return NAN;

    if (!isfinite(x) || !isfinite(y))
        return NAN;

    prediction = linRPredict(r, x);

    if (!isfinite(prediction))
        return NAN;

    return y - prediction;
}

double linRErrorSquare(const Result *r)
{
    double rss;

    if (!r || !r->calculated)
        return NAN;

    rss = r->Syy - r->m * r->Sxy;

    if (rss < 0.0 && fabs(rss) < LINR_EPSILON)
        rss = 0.0;

    return rss;
}

double linRTotalSquare(const Result *r)
{
    if (!r || !r->n)
        return NAN;

    return r->Syy;
}

double linRR2(const Result *r)
{
    double rss, tss;

    if (!r || !r->calculated)
        return NAN;

    rss = linRErrorSquare(r);
    tss = linRTotalSquare(r);

    if (!isfinite(rss) || !isfinite(tss))
        return NAN;

    if (fabs(tss) <= LINR_EPSILON)
        return NAN;

    return 1.0 - rss / tss;
}

void linRShow(const Result *r)
{
    if (!r || !r->calculated)
        return;

    printf("y = %lfx + %lf\n", r->m, r->b);
}
