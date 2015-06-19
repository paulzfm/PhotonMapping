#include "Plane.h"

const std::string Plane::CLS = "objects:plane";

bool Plane::hit(const Ray& ray, double tmin, double tmax, 
    double time, HitRecord& record) const
{
    double denom = n.dot(ray.d);
    if (denom != 0) {
        double t = -(n.dot(ray.o) + d) / denom;
        if (t < 0) {
            return false;
        }

        // In valid interval?
        if (t < tmin || t > tmax) {
            return false;
        }

        // Hit! Let's record it.
        record.t = t;
        record.n = n;
        record.color = color;
        return true;
    }

    return false;
}

std::shared_ptr<Shape> Plane::parse(const JsonBox::Value& val)
{
    Parser::checkObject(val, CLS);

    JsonBox::Object obj = val.getObject();
    Parser::checkParam(obj, CLS, "normal", Parser::VEC3);
    Parser::checkParam(obj, CLS, "distance", Parser::NUMBER);

    return std::shared_ptr<Plane>(new Plane(
        Parser::asVector(obj["normal"]), 
        Parser::asNumber(obj["distance"])
    ));
}