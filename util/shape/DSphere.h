#ifndef DSPHERE_H_
#define DSPHERE_H_

#include "Sphere.h"

// Dynamic Sphere

class DSphere : public Sphere
{
public:
	DSphere(const Vector& center, double radius, float tmin, float tmax) 
        : Sphere(center, radius), _tmin(tmin), _tmax(tmax) {}

    bool hit(const Ray& ray, double tmin, double tmax, 
        double time, HitRecord& record) const;
    bool shadowHit(const Ray& ray, double tmin, double tmax, 
        double time) const;

    Vector center(double time) const;
	
private:
    double _tmin, _tmax; // time range
};

#endif // DSPHERE_H_