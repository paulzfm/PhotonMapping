#ifndef POINT_H_
#define POINT_H_

#include <iostream>

struct Point
{
    Point(double x = 0, double y = 0)
        : x(x), y(y) {}

    double x;
    double y;

    friend std::ostream& operator << (std::ostream& os, const Point &p);
};

#endif // POINT_H_