#include "Triangle.h"
#include "../common.h"

// Möller–Trumbore intersection algorithm
// refer to:
//     https://en.wikipedia.org/wiki/Möller–Trumbore_intersection_algorithm
bool Triangle::hit(const Ray& ray, double time, HitRecord& record) const
{
    Vector e1 = v2 - v1;
    Vector e2 = v3 - v1;
    Vector p = ray.d.cross(e2);
    
    double det = e1.dot(p);
    if (det > -EPS && det < EPS) {
        return false;
    }
    double inv_det = 1.0 / det;

    Vector t = ray.o - v1;
    double u = t.dot(p) * inv_det;
    if (u < 0.0 || u > 1.0) {
        return false;
    }

    Vector q = t.cross(e1);
    double v = ray.d.dot(q) * inv_det;
    if (v < 0.0 || v > 1.0 || u + v > 1.0) {
        return false;
    }

    record.t = e2.dot(q) * inv_det;
    if (record.t < EPS) {
        return false;
    }

    // Hit!
    record.n = e1.cross(e2).normalize();
    return true;
}

RGB Triangle::colorAt(const Vector& pos) const
{
    return color;
}

std::shared_ptr<Shape> Triangle::parse(const JsonBox::Value& val,
    const std::string& CLS)
{
    Parser::checkObject(val, CLS);

    JsonBox::Object obj = val.getObject();
    Parser::checkParam(obj, CLS, "v1", Parser::VEC3);
    Parser::checkParam(obj, CLS, "v2", Parser::VEC3);
    Parser::checkParam(obj, CLS, "v3", Parser::VEC3);

    return std::shared_ptr<Shape>(new Triangle(
        Parser::asVector(obj["v1"]),
        Parser::asVector(obj["v2"]),
        Parser::asVector(obj["v3"])
    ));
}

BBox Triangle::boundingBox() const
{
    const double eps = 0.000001;
    Vector min, max;

    min.x = MIN(v1.x, v2.x);
    min.y = MIN(v1.y, v2.y);
    min.z = MIN(v1.z, v2.z);

    min.x = MIN(v3.x, min.x);
    min.y = MIN(v3.y, min.y);
    min.z = MIN(v3.z, min.z);

    max.x = MAX(v1.x, v2.x);
    max.y = MAX(v1.y, v2.y);
    max.z = MAX(v1.z, v2.z);

    max.x = MAX(v3.x, max.x);
    max.y = MAX(v3.y, max.y);
    max.z = MAX(v3.z, max.z);

    min.x -= eps;
    min.y -= eps;
    min.z -= eps;

    max.x += eps;
    max.y += eps;
    max.z += eps;

    return BBox(min, max);
}

std::ostream& operator << (std::ostream& os, const Triangle& tri)
{
    os << tri.v1 << ", " << tri.v2 << ", " << tri.v3;
    return os;
}

