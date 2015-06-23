#ifndef UTIL_SHAPE_PLANE_H
#define UTIL_SHAPE_PLANE_H

#include "Shape.h"

class Plane : public Shape
{
public:
    Plane(const Vector& normal, const Vector& point)
        : n(normal), p(point) {}

    static std::shared_ptr<Shape> parse(const JsonBox::Value& val,
        const std::string& CLS);

    bool hit(const Ray& ray, double time, HitRecord& record) const;

    Vector n; // normal
    Vector p; // point
};

#endif