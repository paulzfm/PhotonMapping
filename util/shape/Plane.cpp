#include "Plane.h"

bool Plane::hit(const Ray& ray, double time, HitRecord& record) const
{
    double a = n.dot(ray.d);
    if (a == 0) {
        return false;
    }

    double b = n.dot(p - ray.o);
    if (a * b < EPS) {
        return false;
    }

    // Hit! Let's record it.
    record.t = b / a;
    record.n = n;
    return true;
}

RGB Plane::colorAt(const Vector& pos) const
{
    return color;
}

std::shared_ptr<Shape> Plane::parse(const JsonBox::Value& val, 
    const std::string& CLS)
{
    Parser::checkObject(val, CLS);

    JsonBox::Object obj = val.getObject();
    Parser::checkParam(obj, CLS, "normal", Parser::VEC3);
    Parser::checkParam(obj, CLS, "point", Parser::VEC3);

    return std::shared_ptr<Plane>(new Plane(
        Parser::asVector(obj["normal"]), 
        Parser::asVector(obj["point"])
    ));
}