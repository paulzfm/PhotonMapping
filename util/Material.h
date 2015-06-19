#ifndef UTIL_MATERIAL_H_
#define UTIL_MATERIAL_H_

#include "RGB.h"
#include "Parser.h"

#define DEFAULT_IND 1
#define DEFAULT_ABS 1
#define DEFAULT_ROU 1
#define DEFAULT_REF 1
#define DEFAULT_EMI 1

class Material
{
public:
    Material() :
        index_of_refraction(DEFAULT_IND), absorvance(DEFAULT_ABS),
        roughness(DEFAULT_ROU), reflectance(DEFAULT_REF),
        emittance(DEFAULT_EMI) {}

    RGB color;

    double index_of_refraction; // index of refraction
    // double specular_power;  // specular power: (R.V)^n
    double absorvance;
    double roughness;
    double reflectance;
    double emittance;

    static Material parse(const JsonBox::Value& val);

    const static std::string CLS;
};

#endif // UTIL_MATERIAL_H_