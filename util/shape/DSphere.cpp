#include "DSphere.h"

bool DSphere::hit(const Ray& ray, double tmin, double tmax, 
    double time, HitRecord& record) const
{
    Vector newc = center(time);

    double A = ray.d.dot(ray.d);
    Vector OC = ray.o - newc;
    double B = ray.d.dot(OC);
    double C = OC.dot(OC) - _sqr;
    double delta = B * B - A * C;
    if (delta <= 0) {
        return false;
    }

    double t = (-B - sqrt(delta)) / A;

    // In valid interval?
    if (t < tmin) {
        t = (-B + sqrt(delta)) / A;
    }
    if (t < tmin || t > tmax) {
        return false;
    }

    // Hit! Let's record it.
    record.t = t;
    record.n = (ray.o + ray.d * t - newc).normalize();
    return true;
}

Vector DSphere::center(double time) const
{
    double offset = _tmax * time + _tmin * (1.0 - time);
    return Vector(c.x + offset, c.y + offset, c.z + offset);
}

RGB DSphere::colorAt(const Vector& pos) const
{
    if (_texture) {
        return RGB();
    } else {
        return color;
    }
}
