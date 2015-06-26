// Base class of Shape

#ifndef UTIL_SHAPE_H_
#define UTIL_SHAPE_H_

#include "../common.h"
#include "../Ray.h"
#include "../RGB.h"
#include "../Parser.h"
#include "../Material.h"
#include "../Texture.h"

#include <string>
#include <memory>
#include <math.h>

struct HitRecord
{
    double t;
    Vector n;
};

class Shape
{
public:
    void setMaterial(const Material& m);

    virtual bool hit(const Ray& ray, double time, HitRecord& record) const = 0;
    
    virtual RGB colorAt(const Vector& pos) const = 0;

    // params
    RGB color;

    double index_of_refraction; // index of refraction
    // double specular_power;  // specular power: (R.V)^n
    double absorvance;
    double roughness;
    double emittance;

protected:
    std::unique_ptr<Texture> _texture;
};

#endif // UTIL_SHAPE_H_