#ifndef TRIANGLE_H_
#define TRIANGLE_H_

#include "Shape.h"

class Triangle : public Shape
{
public:
    Triangle(const Vector& v1, const Vector& v2, const Vector& v3)
        : v1(v1), v2(v2), v3(v3) {}

    bool hit(const Ray& ray, double tmin, double tmax, 
        double time, HitRecord& record) const;
    bool shadowHit(const Ray& ray, double tmin, double tmax, 
        double time) const;

    Vector v1;
    Vector v2;
    Vector v3;
};

#endif // TRIANGLE_H_