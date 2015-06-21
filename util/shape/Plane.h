#ifndef UTIL_SHAPE_PLANE_H
#define UTIL_SHAPE_PLANE_H

#include "Shape.h"

class Plane : public Shape
{
public:
    Plane(const Vector& normal, double distance)
        : n(normal), d(distance) {}

    static std::shared_ptr<Shape> parse(const JsonBox::Value& val);

    bool hit(const Ray& ray, double tmin, double tmax, 
        double time, HitRecord& record) const;

    Vector n;
    double d;

    const static std::string CLS;
};

#endif