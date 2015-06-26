#ifndef UTIL_LIGHT_LIGHT_H
#define UTIL_LIGHT_LIGHT_H

#include "Shape.h"
#include "../Ray.h"
#include "../Vector.h"
#include "../PhotonMap.h"

#include <stdlib.h>

class Light : public Shape
{
public:
    virtual Photon randomPhoton() const = 0;

    virtual bool hit(const Ray& ray, double time, HitRecord& record) const = 0;

    virtual double area() const = 0;
};

#endif // UTIL_LIGHT_LIGHT_H