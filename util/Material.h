#ifndef UTIL_MATERIAL_H_
#define UTIL_MATERIAL_H_

#include "RGB.h"
#include "Parser.h"

class Material
{
public:
    Material(double absorvance = 0, double roughness = 0, double emittance = 0, 
        double index_of_refraction = 1, const RGB& color = RGB())
        : index_of_refraction(index_of_refraction),
          absorvance(absorvance),
          roughness(roughness),
          emittance(emittance) {}

    RGB color;

    double index_of_refraction; // index of refraction
    // double specular_power;  // specular power: (R.V)^n
    double absorvance;
    double roughness;
    double emittance;

    static Material parse(const JsonBox::Value& val);

    const static std::string CLS;
};

#endif // UTIL_MATERIAL_H_