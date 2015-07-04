#ifndef UTIL_BOX_H_
#define UTIL_BOX_H_

#include "Quad.h"
#include "Shape.h"

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

class Box : public Shape
{
public:
    Box(const Vector& s, const Vector& a, const Vector& b, const Vector& c);

    static std::shared_ptr<Shape> parse(const JsonBox::Value& val, 
        const std::string& CLS);

    bool hit(const Ray& ray, double time, HitRecord& record) const;

    RGB colorAt(const Vector& pos) const;

    // @override
    void setMaterial(const Material& m);

private:
    Vector _a, _b, _c;
    std::vector<Quad> _quads; // 6 faces
};

#endif // UTIL_BOX_H_