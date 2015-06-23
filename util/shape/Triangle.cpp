#include "Triangle.h"

bool Triangle::hit(const Ray& ray, double time, HitRecord& record) const
{
    double t;
    double A = v1.x - v2.x;
    double B = v1.y - v2.y;
    double C = v1.z - v2.z;

    double D = v1.x - v3.x;
    double E = v1.y - v3.y;
    double F = v1.z - v3.z;

    double G = ray.d.x;
    double H = ray.d.y;
    double I = ray.d.z;

    double J = v1.x - ray.o.x;
    double K = v1.y - ray.o.y;
    double L = v1.z - ray.o.z;

    double EIHF = E * I - H * F;
    double GFDI = G * F - D * I;
    double DHEG = D * H - E * G;
    double alpha = (A * EIHF + B * GFDI + C * DHEG);
    double beta = (J * EIHF + K * GFDI + L * DHEG) / alpha;

    if (beta <= 0.0 || beta >= 1.0) {
        return false;
    }

    double AKJB = A * K - J * B;
    double JCAL = J * C - A * L;
    double BLKC = B * L - K * C;
    double gamma = (I * AKJB + H * JCAL + G * BLKC) / alpha;

    if (gamma <= 0.0 || beta + gamma >= 1.0) {
        return false;
    }

    t = -(F * AKJB + E * JCAL + D * BLKC) / alpha;

    // Hit! Let's record it.
    record.t = t;
    record.n = (v2 - v1).cross(v3 - v1).normalize();
    return true;
}

RGB Triangle::colorAt(const Vector& pos) const
{
    if (_texture) {
        return RGB();
    } else {
        return color;
    }
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
