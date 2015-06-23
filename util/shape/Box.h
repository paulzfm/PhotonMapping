#ifndef UTIL_LIGHT_BOX
#define UTIL_LIGHT_BOX

#include "Light.h"
#include "Quad.h"

/* A box defined as

         +------------+
        /|           /|
       / |          / |
      /  |         /  |
     /   |        /   |
    A------------+    |
    |    |       |    |
    |    B------------+
    |   /        |   /
    |  /         |  /
    | /          | /
    |/           |/
    S------------C

    S = s
    A = S + a
    B = S + b
    C = S + c
*/

class Box : public Light
{
public:
    Box(const Vector& s, const Vector& a, const Vector& b, const Vector& c);

    static std::shared_ptr<Light> parse(const JsonBox::Value& val, 
        const std::string& CLS);

    bool hit(const Ray& ray, double time, HitRecord& record) const;

    Photon randomPhoton() const;

    double area() const;

private:
    Vector _a, _b, _c;
    std::vector<Quad> _quads; // 6 faces
    double _area;
};

#endif