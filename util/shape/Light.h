#ifndef UTIL_LIGHT_LIGHT_H
#define UTIL_LIGHT_LIGHT_H

#include "Shape.h"
#include "../Ray.h"
#include "../Vector.h"

#include <stdlib.h>

class Light : public Shape
{
public:
    virtual Ray randomRay() const = 0;

    virtual bool hit(const Ray& ray, double tmin, double tmax, 
        double time, HitRecord& record) const = 0;

    virtual double area() const = 0;

    Vector pos;
};

#endif // UTIL_LIGHT_LIGHT_H