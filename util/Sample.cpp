#include "Sample.h"
#include <math.h>
#include <stdlib.h>

int Sample::randint(int min, int max)
{
    return min + int(drand48() * (max - min));
}

void Sample::jitter(Point* samples, int n)
{
    int m = sqrt(n);
    for (int i = 0; i < m; i++) {
        for (int j = 0; j < m; j++) {
            samples[i * m + j].x = ((double)i + drand48()) / m;
            samples[i * m + j].y = ((double)j + drand48()) / m;
        }
    }
}

void Sample::jitter(double* samples, int n)
{
    for (int i = 0; i < n; i++) {
        samples[i] = ((double)i + drand48()) / n;
    }
}

void Sample::multiJitter(Point* samples, int n)
{
    int m = sqrt(n);
    double subcell = 1.0 / n;

    // initialize
    for (int i = 0; i < m; i++) {
        for (int j = 0; j < m; j++) {
            samples[i * m + j].x = i / (double)m + ((double)j + drand48()) * subcell;
            samples[i * m + j].y = j / (double)m + ((double)i + drand48()) * subcell;
        }
    }

    // shuffle
    for (int i = 0; i < m; i++) {
        for (int j = 0; j < m; j++) {
            int k = randint(j, m - 1);
            double tmp = samples[i * m + j].x;
            samples[i * m + j].x = samples[i * m + k].x;
            samples[i * m + k].x = tmp;

            k = randint(j, m - 1);
            tmp = samples[j * m + i].y;
            samples[j * m + i].y = samples[k * m + i].y;
            samples[k * m + i].y = tmp;
        }
    }
}

void Sample::boxFilter(Point* samples, int n)
{
    for (int i = 0; i < n; i++) {
        samples[i].x -= 0.5;
        samples[i].y -= 0.5;
    }
}

void Sample::tentFilter(Point* samples, int n)
{
    for (int i = 0; i < n; i++) {
        if (samples[i].x < 0.5) {
            samples[i].x = sqrt(2.0 * samples[i].x) - 1.0;
        } else {
            samples[i].x = 1.0 - sqrt(2.0 - 2.0 * samples[i].x);
        }

        if (samples[i].y < 0.5) {
            samples[i].y = sqrt(2.0 * samples[i].y) - 1.0;
        } else {
            samples[i].y = 1.0 - sqrt(2.0 - 2.0 * samples[i].y);
        }
    }
}

