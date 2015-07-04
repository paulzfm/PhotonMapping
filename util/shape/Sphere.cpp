#include "Sphere.h"

bool Sphere::hit(const Ray& ray, double time, HitRecord& record) const
{
    Vector CO = c - ray.o;
    double A = CO.dot(ray.d);
    
    if (A < 0.0) {
        return false;
    }

    double d2 = CO.dot(CO) - A * A; // pitagora's theorem
    if (d2 > _sqr) {
        return false;
    }

    double C = sqrt(_sqr - d2);
    double t = A - C;
    if (t > EPS || t > -EPS) {
        record.t = t;
    } else {
        record.t = A + C;
    }

    record.n = (ray.o + record.t * ray.d - c) / r;
    return true;
}

RGB Sphere::colorAt(const Vector& pos) const
{
    if (_texture) {
        Vector n = (pos - c) / r;

        // calculate UV coordinates
        double theta = acos(0.9999 * n.z);
        double sin_theta = sin(theta);
        double phi = acos(n.x / (1.0001 * sin_theta));
        if (n.y < EPS) {
            phi = 2 * PI - phi;
        }

        Point p(phi / (2.0 * PI), 1.0 - theta / PI);
        return _texture->value(p);
    } else {
        return color;
    }
}

std::shared_ptr<Shape> Sphere::parse(const JsonBox::Value& val, 
    const std::string& CLS)
{
    Parser::checkObject(val, CLS);

    JsonBox::Object obj = val.getObject();
    Parser::checkParam(obj, CLS, "center", Parser::VEC3);
    Parser::checkParam(obj, CLS, "radius", Parser::NUMBER);

    return std::shared_ptr<Shape>(new Sphere(
        Parser::asVector(obj["center"]), 
        Parser::asNumber(obj["radius"])
    ));
}
