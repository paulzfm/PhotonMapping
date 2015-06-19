#ifndef SPHERE_H_
#define SPHERE_H_

#include "Shape.h"
#include "../Parser.h"

class Sphere : public Shape
{
public:
	Sphere(const Vector& center, double radius) 
        : c(center), r(radius) { _sqr = r * r; }

    static std::shared_ptr<Shape> parse(const JsonBox::Value& val);

    bool hit(const Ray& ray, double tmin, double tmax, 
        double time, HitRecord& record) const;
    bool shadowHit(const Ray& ray, double tmin, double tmax, 
        double time) const;

    Vector c;
    double r;

    const static std::string CLS;
	
protected:
	double _sqr; // r ^ 2
};

#endif // SPHERE_H_