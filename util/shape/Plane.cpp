#include "Plane.h"

bool Plane::hit(const Ray& ray, double tmin, double tmax, 
    double time, HitRecord& record) const
{
    double denom = n.dot(ray.d);
    if (denom != 0.0) {
        double t = -(n.dot(ray.o) + d) / denom;

        // In valid interval?
        if (t < tmin || t > tmax) {
            return false;
        }

        // Hit! Let's record it.
        record.t = t;
        record.n = n;
        record.color = color;
        return true;
    }

    return false;
}