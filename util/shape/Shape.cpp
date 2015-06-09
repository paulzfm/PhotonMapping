#include "Shape.h"
#include "../common.h"

/*
Ray Shape::reflect(const Ray& incidence, HitRecord& record)
{
    return Ray(
        incidence.vectorAt(record.t),
        incidence.d - 2 * (incidence.d.dot(record.n)) * record.n
    );
}

Ray Shape::refract(const Ray& incidence, HitRecord& record)
{
    double a = incidence.air ? (1.0 / index_of_refraction) : index_of_refraction;
    double cos_i = Vector::cos(-incidence.d, record.n);
    double cos_t = sqrt(1 - a * a * (1 - cos_i * cos_i));

    return Ray(
        incidence.vectorAt(record.t),
        -a * incidence.d + (a * incidence.d.dot(record.n) - cos_t) * record.n
    );
}

RGB Shape::lambert(const Ray& incidence, HitRecord& record, const RGB& light)
{
    return light * ABS(record.n.dot(-incidence.d)) * diffuse_factor;
}

RGB Shape::phong(const Ray& incidence, HitRecord& record, const RGB& light, 
    const Vector& view)
{
    RGB color = lambert(incidence, record, light);
    
    double RV = reflect(incidence, record).d.dot(-view); // R . V
    if (RV <= 0) {
        return color;
    }

    color += pow(RV, specular_power) * light * specular_factor;
    return color;
}
*/