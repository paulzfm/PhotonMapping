#include "Sphere.h"

bool Sphere::hit(const Ray& ray, double tmin, double tmax, 
    double time, HitRecord& record) const
{
    double A = ray.d.dot(ray.d);
    Vector OC = ray.o - c;
    double B = ray.d.dot(OC);
    double C = OC.dot(OC) - _sqr;
    double delta = B * B - A * C;
    if (delta <= 0) {
        return false;
    }

    double t = (-B - sqrt(delta)) / A;
    if (t < tmin) {
        t = (-B + sqrt(delta)) / A;
    }

    // In valid interval?
    if (t < tmin || t > tmax) {
        return false;
    }

    // Hit! Let's record it.
    record.t = t;
    record.n = (ray.o + ray.d * t - c).normalize();
    record.color = color;
    return true;
}

bool Sphere::shadowHit(const Ray& ray, double tmin, double tmax, 
    double time) const
{
    double A = ray.d.dot(ray.d);
    Vector OC = ray.o - c;
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

    // Hit!
    return true;
}