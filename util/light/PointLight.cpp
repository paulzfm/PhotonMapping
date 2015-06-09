#include "PointLight.h"

Ray PointLight::randomRay() const
{
    double x, y, z;
    do {
        x = -1.0 + 2.0 * drand48();
        y = -1.0 + 2.0 * drand48();
        z = -1.0 + drand48();
    } while(x * x + y * y + z * z > 1.0);

    return Ray(pos, Vector(x, y, z));
}