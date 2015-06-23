#ifndef SPHERE_H_
#define SPHERE_H_

#include "Shape.h"
#include "../Parser.h"

class Sphere : public Shape
{
public:
	Sphere(const Vector& center, double radius) 
        : c(center), r(radius) { _sqr = r * r; }

    static std::shared_ptr<Shape> parse(const JsonBox::Value& val,
        const std::string& CLS);

    bool hit(const Ray& ray, double time, HitRecord& record) const;

    RGB colorAt(const Vector& pos) const;

    Vector c;
    double r;
	
protected:
	double _sqr; // r ^ 2
};

#endif // SPHERE_H_