#ifndef UTIL_LIGHT_POINT_LIGHT_H
#define UTIL_LIGHT_POINT_LIGHT_H

#include "Light.h"

class PointLight : public Light
{
public:
    Ray randomRay() const;
    Ray randomRay2() const;

    Vector pos;
};

#endif // UTIL_LIGHT_POINT_LIGHT_H