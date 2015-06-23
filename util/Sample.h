#ifndef SAMPLE_H_
#define SAMPLE_H_

#include "Point.h"
#include "common.h"

#include <vector>

class Sample
{
public:
    // 2D sampling: assume that n is a perfect square integer,
    // and m * m = n, then we generate m * m samples.
    static void jitter(std::vector<Point>& samples, int n);
    // static void multiJitter(std::vector<Point> samples, int n);
    
    // static void boxFilter(std::vector<Point> samples, int n);
    // static void tentFilter(std::vector<Point> samples, int n);

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