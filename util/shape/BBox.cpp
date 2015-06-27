#include "BBox.h"

#include <limits>

bool BBox::shadowHit(const Ray& ray) const
{
    int posneg[3];
    posneg[0] = (ray.d.x > 0 ? 0 : 1);
    posneg[1] = (ray.d.y > 0 ? 0 : 1);
    posneg[2] = (ray.d.z > 0 ? 0 : 1);
    Vector inv(1.0 / ray.d.x, 1.0 / ray.d.y, 1.0 / ray.d.z);

    double interval_min = -std::numeric_limits<double>::max();
    double interval_max =  std::numeric_limits<double>::max();

    double t0 = (p[posneg[0]].x - ray.o.x) * inv.x;
    double t1 = (p[1 - posneg[0]].x - ray.o.x) * inv.x;
    if (t0 > interval_min) interval_min = t0;
    if (t1 < interval_max) interval_max = t1;
    if (interval_min > interval_max) return false;

    t0 = (p[posneg[1]].y - ray.o.y) * inv.y;
    t1 = (p[1 - posneg[1]].y - ray.o.y) * inv.y;
    if (t0 > interval_min) interval_min = t0;
    if (t1 < interval_max) interval_max = t1;
    if (interval_min > interval_max) return false;

    t0 = (p[posneg[2]].z - ray.o.z) * inv.z;
    t1 = (p[1 - posneg[2]].z - ray.o.z) * inv.z;
    if (t0 > interval_min) interval_min = t0;
    if (t1 < interval_max) interval_max = t1;
    return (interval_min <= interval_max);
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

