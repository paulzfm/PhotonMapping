#include "PointLight.h"

Ray PointLight::randomRay() const
{
    double x, y, z;
    do {
        x = -1.0 + 2.0 * drand48();
        y = -1.0 + 2.0 * drand48();
        z = -1.0 + drand48();
    } while(x * x + y * y + z * z > 1.0);

    return Ray(pos, Vector(x, y, z).normalize());
}

Ray PointLight::randomRay2() const
{
    double x, y, z;
    x = -1.0 + 2.0 * drand48();
    y = -1.0 + 2.0 * drand48();
    z = -1.0 + 2.0 * drand48();

    return Ray(pos, Vector(x, y, z));
}

std::shared_ptr<Light> PointLight::parse(const JsonBox::Value& val)
{
    return nullptr;
}