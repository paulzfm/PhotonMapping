#ifndef UTIL_LIGHT_LIGHT_H
#define UTIL_LIGHT_LIGHT_H

#include "../Ray.h"
#include "../Vector.h"
#include <stdlib.h>

class Light
{
public:
    virtual Ray randomRay() const = 0;

    Vector pos;
};

#endif // UTIL_LIGHT_LIGHT_H