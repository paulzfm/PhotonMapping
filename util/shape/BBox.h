#ifndef UTIL_SHAPE_BBOX
#define UTIL_SHAPE_BBOX

#include "Shape.h"

class BBox : public Shape
{
public:
    // a, b are left-bottom and right-top points
    BBox(const Vector& a = Vector(), const Vector& b = Vector())
    {
        p[0] = a;
        p[1] = b;
    }

    // no records will be returned: do NOT use this
    bool hit(const Ray& ray, double time, HitRecord& record) const;

    // use this to test intersection
    bool shadowHit(const Ray& ray) const;

    // will always return black
    RGB colorAt(const Vector& pos) const;

    // merge two small bbox into a larger one
    void expand(const BBox& that);

    // longest axis
    int longestAxis() const;
    // return:
    const static int X = 1;
    const static int Y = 2;
    const static int Z = 3;

    friend std::ostream& operator << (std::ostream& os, const BBox& bbox);

    Vector p[2];
};

#endif // UTIL_SHAPE_BBOX