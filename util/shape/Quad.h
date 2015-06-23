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

    static std::shared_ptr<Light> parse(const JsonBox::Value& val,
        const std::string& CLS);

    bool hit(const Ray& ray, double time, HitRecord& record) const;

    Ray randomRay() const;

    Photon randomPhoton() const;

    double area() const;

    RGB colorAt(const Vector& pos) const;

    Vector A;
    Vector B;
    Vector C;
    Vector normal;

private:
    Vector _u, _v;
    double _area;
};

#endif // UTIL_LIGHT_QUAD