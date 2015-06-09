#ifndef UTIL_SHAPE_PLANE_H
#define UTIL_SHAPE_PLANE_H

#include "Shape.h"

class Plane : public Shape
{
public:
    Plane(const Vector& normal, double d)
        : n(normal), d(d) { reflective = 0.0; }

    bool hit(const Ray& ray, double tmin, double tmax, 
        double time, HitRecord& record) const;

    Vector n;
    double d;
};

#endif