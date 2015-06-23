#ifndef TRIANGLE_H_
#define TRIANGLE_H_

#include "Shape.h"

class Triangle : public Shape
{
public:
    Triangle(const Vector& v1, const Vector& v2, const Vector& v3)
        : v1(v1), v2(v2), v3(v3) {}

    static std::shared_ptr<Shape> parse(const JsonBox::Value& val,
        const std::string& CLS);

    bool hit(const Ray& ray, double time, HitRecord& record) const;

    RGB colorAt(const Vector& pos) const;

    Vector v1;
    Vector v2;
    Vector v3;
};

#endif // TRIANGLE_H_