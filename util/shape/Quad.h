#ifndef UTIL_LIGHT_QUAD
#define UTIL_LIGHT_QUAD

#include "Light.h"

/* A planar convex quadrilateral defined as

         B------------+
        /            /
       /            /
      /            /
     /            /
    A------------C

 */

class Quad : public Light
{
public:
    Quad(const Vector& a, const Vector& b, const Vector& c);

    static std::shared_ptr<Light> parse(const JsonBox::Value& val);

    bool hit(const Ray& ray, double tmin, double tmax, 
        double time, HitRecord& record) const;

    Ray randomRay() const;

    double area() const;

    Vector A;
    Vector B;
    Vector C;

    const static std::string CLS;

private:
    Vector _u, _v, _n;
    double _area;
};

#endif // UTIL_LIGHT_QUAD