#include "Quad.h"

const std::string Quad::CLS = "lights:quad";

Quad::Quad(const Vector& a, const Vector& b, const Vector& c)
    : A(a), B(b), C(c)
{
    _u = b - a;
    _v = c - a;
    _area = _u.cross(_v).length();
    _n = _u.cross(_v).normalize();
}

Ray Quad::randomRay() const
{
    double alpha = drand48();
    double beta = drand48();

    return Ray(
        A + _u * alpha + _v * beta,
        _n.noise(roughness)
    );
}

bool Quad::hit(const Ray& ray, double tmin, double tmax, 
    double time, HitRecord& record) const
{
    double a = _n.dot(ray.d);
    if (a == 0) /* Ray and Plane are parallel -> no intersection or Line intersection FIXME */
        return false;

    double b = _n.dot(A - ray.o);
    if (a * b < 0) /* point is behind the ray's origin */
        return false;

    /* 
        o is the vector from A to P
        point P is inside only if the projections of o onto u (and v) are between 0 and |u| (and |v| respectivly)
        
        0 <= |o|*cos(theta) <= |u|
        0 <= |o|* (o . u)/(|o|*|u|) <= |u|
        0 <= (o . u) / |u| <= |u|
        0 <= o. u <= |u|^2  
    */
    Vector o = ray.o + ray.d * (b / a) - A;
    double dot = _u.dot(o);
    if (dot < 0 || dot > _u.square())
        return false;

    dot = _v.dot(o);
    if (dot < 0 || dot > _v.square())
        return false;


    record.t = b / a;
    record.n = _n;
    record.color = color;
    return true;
}

double Quad::area() const
{
    return _area;
}

std::shared_ptr<Light> Quad::parse(const JsonBox::Value& val)
{
    Parser::checkObject(val, CLS);

    JsonBox::Object obj = val.getObject();
    Parser::checkParam(obj, CLS, "A", Parser::VEC3);
    Parser::checkParam(obj, CLS, "B", Parser::VEC3);
    Parser::checkParam(obj, CLS, "C", Parser::VEC3);

    return std::shared_ptr<Light>(new Quad(
        Parser::asVector(obj["A"]), 
        Parser::asVector(obj["B"]), 
        Parser::asVector(obj["C"])
    ));
}
