#include "BBox.h"

#include <limits>
#include <assert.h>

// We refer to an efficient and robust algorithm to adapt to special 
// cases described in the following paper:
//    Amy Williams, Steve Barrus, R. Keith Morley, and Peter Shirley
//    "An Efficient and Robust Ray-Box Intersection Algorithm"
//    Journal of graphics tools, 10(1):49-54, 2005
// Here is the pdf format paper:
//    http://www.cs.utah.edu/~awilliam/box/box.pdf
bool BBox::shadowHit(const Ray& ray) const
{
    Vector inv_direction = Vector(1.0 / ray.d.x, 1.0 / ray.d.y, 1.0 / ray.d.z);
    int sign[3];
    sign[0] = (inv_direction.x < 0);
    sign[1] = (inv_direction.y < 0);
    sign[2] = (inv_direction.z < 0);

    double tmin = (p[sign[0]].x - ray.o.x) * inv_direction.x;
    double tmax = (p[1 - sign[0]].x - ray.o.x) * inv_direction.x;
    double tymin = (p[sign[1]].y - ray.o.y) * inv_direction.y;
    double tymax = (p[1 - sign[1]].y - ray.o.y) * inv_direction.y;
    if ((tmin > tymax) || (tymin > tmax)) return false;
    if (tymin > tmin) tmin = tymin;
    if (tymax < tmax) tmax = tymax;
    double tzmin = (p[sign[2]].z - ray.o.z) * inv_direction.z;
    double tzmax = (p[1 - sign[2]].z - ray.o.z) * inv_direction.z;
    if ((tmin > tzmax) || (tzmin > tmax)) return false;
    if (tzmin > tmin) tmin = tzmin;
    if (tzmax < tmax) tmax = tzmax;
    return true;
}

bool BBox::hit(const Ray& ray, double time, HitRecord& record) const
{
    return shadowHit(ray);
}

RGB BBox::colorAt(const Vector& pos) const
{
    return color;
}

void BBox::expand(const BBox& that)
{
    p[0] = Vector(
        MIN(p[0].x, that.p[0].x),
        MIN(p[0].y, that.p[0].y),
        MIN(p[0].z, that.p[0].z)
    );

    p[1] = Vector(
        MAX(p[1].x, that.p[1].x),
        MAX(p[1].y, that.p[1].y),
        MAX(p[1].z, that.p[1].z)
    );
}

int BBox::longestAxis() const
{
    double x = p[1].x - p[0].x;
    double y = p[1].y - p[0].y;
    double z = p[1].z - p[0].z;

    if (x >= y && x >= z) return X;
    if (y >= x && y >= z) return Y;
    return Z;
}

std::ostream& operator << (std::ostream& os, const BBox& bbox)
{
    os << "[" << bbox.p[0] << ", " << bbox.p[1] << "]";
    return os;
}

