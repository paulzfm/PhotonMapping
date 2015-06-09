// Base class of Shape

#ifndef SHAPE_H_
#define SHAPE_H_

#include "../Ray.h"
#include "../RGB.h"
#include <Math.h>

struct HitRecord
{
    double t;
    Vector n;
    RGB color;
};

class Shape
{
public:
    virtual bool hit(const Ray& ray, double tmin, double tmax, 
        double time, HitRecord& record) const = 0;
    // virtual bool shadowHit(const Ray& ray, double tmin, double tmax, 
        // double time) const = 0;

    /*
    Ray reflect(const Ray& incidence, HitRecord& record);
    Ray refract(const Ray& incidence, HitRecord& record);
    RGB lambert(const Ray& incidence, HitRecord& record, const RGB& light);
    RGB phong(const Ray& incidence, HitRecord& record, const RGB& light, 
        const Vector& view);
    */

    // params
    RGB color;

    double index_of_refraction; // index of refraction
    double specular_power;  // specular power: (R.V)^n
    RGB specular_factor; // specular factor in RGB-channels: Ks
    RGB diffuse_factor;  // diffuse factor in RGB-channels: Kd

    double reflective, refractive;
};

#endif // SHAPE_H_