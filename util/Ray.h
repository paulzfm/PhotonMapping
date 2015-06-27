#ifndef RAY_H_
#define RAY_H_

#include "Vector.h"

class Ray
{
public:
    Ray(const Vector& origin = Vector(), 
        const Vector& direction = Vector(), bool inside = false)
        : o(origin), d(direction), inside(inside) {}
    Ray(const Ray& r) { *this = r; }

    friend std::ostream& operator << (std::ostream& os, const Ray& r);

    Vector o; // origin
    Vector d; // direction
    bool inside; // inside object?

    Vector vectorAt(double t) const { return o + t * d; }  // v = o + td
};

#endif // RAY_H_