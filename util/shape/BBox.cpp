#include "BBox.h"

#include <limits>

bool BBox::shadowHit(const Ray& ray) const
{
    double interval_min = std::numeric_limits<double>::min();
    double interval_max = std::numeric_limits<double>::max();

    double t0 = (p[ray.posneg[0]].x - ray.o.x) * ray.inv.x;
    double t1 = (p[ray.posneg[1]].x - ray.o.x) * ray.inv.x;
    if (t0 > interval_min) interval_min = t0;
    if (t1 < interval_max) interval_max = t1;
    if (interval_min > interval_max) return false;

    t0 = (p[ray.posneg[2]].y - ray.o.y) * ray.inv.y;
    t1 = (p[ray.posneg[3]].y - ray.o.y) * ray.inv.y;
    if (t0 > interval_min) interval_min = t0;
    if (t1 < interval_max) interval_max = t1;
    if (interval_min > interval_max) return false;

    t0 = (p[ray.posneg[4]].z - ray.o.z ) * ray.inv.z;
    t1 = (p[ray.posneg[5]].z - ray.o.z ) * ray.inv.z;
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
        MIN(p[0].y, that.p[0].z)
    );

    p[1] = Vector(
        MAX(p[1].x, that.p[1].x),
        MAX(p[1].y, that.p[1].y),
        MAX(p[1].y, that.p[1].z)
    );
}

int BBox::longestAxis() const
{
    int x = p[1].x - p[0].x;
    int y = p[1].y - p[0].y;
    int z = p[1].z - p[0].z;

    if (x >= y && x >= z) return X;
    if (y >= x && y >= z) return Y;
    return Z;
}


