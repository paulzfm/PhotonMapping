#include "Box.h"
#include "../common.h"

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

    for (const auto& f : _quads) {
        HitRecord tmp;
        if (f.hit(ray, time, tmp)) {
            dis = (-tmp.t * ray.d).length();
            if (!found || dis < min_dis) {
                found = true;
                min_dis = dis;
                record = tmp;
            }
        }
    }

    Vector pos = ray.o + record.t * ray.d;
    for (const auto& f : _quads) {
        if (ABS(f.normal.dot(f.A - pos)) < EPS) {
            record.n = f.normal;
            break;
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
        i = 0;
    } else if (random < a1 + a2) {
        i = (random - a1 < a2 / 2) ? 2 : 4;
    } else {
        i = (random - a1 - a2 < a3 / 2) ? 1 : 3;
    }

    Photon photon = _quads[i].randomPhoton();
    photon.power = color;
    return photon;
}

RGB Box::colorAt(const Vector& pos) const
{
    return color;
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

void Box::setMaterial(const Material& m)
{
    index_of_refraction = m.index_of_refraction;
    absorvance = m.absorvance;
    emittance = m.emittance;
    roughness = m.roughness;
    color = m.color;

    for (auto& f : _quads) {
        f.setMaterial(m);
    }
}
