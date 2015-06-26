#include "Ray.h"

Ray::Ray(const Vector& origin, const Vector& direction, bool inside)
    : o(origin), d(direction), inside(inside),
      inv(1.0 / direction.x, 1.0 / direction.y, 1.0 / direction.z)
{
    posneg[0] = (d.x > 0 ? 0 : 1);
    posneg[1] = (d.y > 0 ? 0 : 1);
    posneg[2] = (d.z > 0 ? 0 : 1);
}

std::ostream& operator << (std::ostream& os, const Ray& r)
{
    os << r.o << "+t" << r.d;
    return os;
}