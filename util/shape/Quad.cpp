#include "Quad.h"

Quad::Quad(const Vector& a, const Vector& b, const Vector& c)
    : A(a), B(b), C(c)
{
    _u = b - a;
    _v = c - a;
    _area = _u.cross(_v).length();
    normal = _u.cross(_v).normalize();
}

Ray Quad::randomRay() const
{
    double alpha = drand48();
    double beta = drand48();

    return Ray(
        A + _u * alpha + _v * beta,
        normal.noise(roughness)
    );
}

Photon Quad::randomPhoton() const
{
    double alpha = drand48();
    double beta = drand48();

    Photon photon;
    photon.ray.o = A + _u * alpha + _v * beta;
    photon.ray.d = normal.noise(roughness);
    photon.power = color;

    return photon;
}

bool Quad::hit(const Ray& ray, double time, HitRecord& record) const
{
    double a = normal.dot(ray.d);
    if (a == 0) {
        return false;
    }

    double b = normal.dot(A - ray.o);
    if (a * b < EPS) {
        return false;
    }

    Vector o = ray.o + ray.d * (b / a) - A;
    double dot = _u.dot(o);
    if (dot < 0 || dot > _u.square()) {
        return false;
    }

    dot = _v.dot(o);
    if (dot < 0 || dot > _v.square()) {
        return false;
    }

    // Hit! Let's record it.
    record.t = b / a;
    record.n = normal;
    return true;
}

double Quad::area() const
{
    return _area;
}

RGB Quad::colorAt(const Vector& pos) const
{
    if (_texture) {
        return RGB();
    } else {
        return color;
    }
}


std::shared_ptr<Light> Quad::parse(const JsonBox::Value& val,
    const std::string& CLS)
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
