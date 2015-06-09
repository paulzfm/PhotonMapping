#ifndef SAMPLE_H_
#define SAMPLE_H_

#include "Point.h"
#include "common.h"

class Sample
{
public:
    // 2D sampling: assume that n is a perfect square integer,
    // and m * m = n, then we generate m * m samples.
    static void jitter(Point* samples, int n);
    static void multiJitter(Point* samples, int n);
    
    static void boxFilter(Point* samples, int n);
    static void tentFilter(Point* samples, int n);

    // 1D sampling
    static void jitter(double* samples, int n);

private:
    Sample();
    ~Sample();
    DISABLE_COPYING(Sample)

    // helper functions
    static int randint(int min, int max);
};

#endif // SAMPLE_H_