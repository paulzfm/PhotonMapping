#ifndef POINT_H_
#define POINT_H_

#include <iostream>

struct Point
{
    double x;
    double y;

    friend std::ostream& operator << (std::ostream& os, const Point &p);
};

#endif // POINT_H_