// Base class of Shape

#ifndef UTIL_SHAPE_H_
#define UTIL_SHAPE_H_

#include "../common.h"
#include "../Ray.h"
#include "../RGB.h"
#include "../Parser.h"
#include "../Material.h"

#include <Math.h>
#include <string>

struct HitRecord
{
    double t;
    Vector n;
    RGB color;
};

class Shape
{
public:
    void setMaterial(const Material& m);

    virtual bool hit(const Ray& ray, double time, HitRecord& record) const = 0;
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
    // double specular_power;  // specular power: (R.V)^n
    double absorvance;
    double roughness;
    double emittance;
};

#endif // UTIL_SHAPE_H_