#include "Box.h"

Box::Box(const Vector& s, const Vector& a, const Vector& b, const Vector& c)
    : _a(a), _b(b), _c(c)
{
    _quads.push_back(Quad(s, s + b, s + c)); // floor
    _quads.push_back(Quad(s + c, s + a + c, s)); // front
    _quads.push_back(Quad(s + c, s + a + c, s + b + c)); // right
    _quads.push_back(Quad(s + b + c, s + b, s + a + b + c)); // back
    _quads.push_back(Quad(s + b, s, s + a + b)); // left
    _quads.push_back(Quad(s + a, s + a + b, s + a + c)); // ceil

    _area = a.length() * b.length() * c.length();
}

bool Box::hit(const Ray& ray, double time, HitRecord& record) const
{
    double min_dis = -1.0;
    double dis;
    bool found = false;

    for (const auto f : _quads) {
        if (f.hit(ray, time, record)) {
            dis = (-record.t * ray.d).length();
            if (!found || dis < min_dis) {
                found = true;
                min_dis = dis;
            }
        }
    }

    return found;
}

double Box::area() const
{
    return _area;
}

Photon Box::randomPhoton() const
{
    double a1 = _b.length() * _c.length();
    double a2 = _a.length() * _b.length();
    double a3 = _a.length() * _c.length();

    double random = drand48() * (a1 + a2 + a3);
    int i;
    if (random < a1) {
        i = (random < a1 / 2) ? 0 : 5;  
    } else if (random < a1 + a2) {
        i = (random - a1 < a2 / 2) ? 2 : 4;
    } else {
        i = (random - a1 - a2 < a3 / 2) ? 1 : 3;
    }

    return _quads[i].randomPhoton();
}

std::shared_ptr<Light> Box::parse(const JsonBox::Value& val,
    const std::string& CLS)
{
    Parser::checkObject(val, CLS);

    JsonBox::Object obj = val.getObject();
    Parser::checkParam(obj, CLS, "s", Parser::VEC3);
    Parser::checkParam(obj, CLS, "a", Parser::VEC3);
    Parser::checkParam(obj, CLS, "b", Parser::VEC3);
    Parser::checkParam(obj, CLS, "c", Parser::VEC3);

    return std::shared_ptr<Light>(new Box(
        Parser::asVector(obj["s"]),
        Parser::asVector(obj["a"]), 
        Parser::asVector(obj["b"]), 
        Parser::asVector(obj["c"])
    ));
}
