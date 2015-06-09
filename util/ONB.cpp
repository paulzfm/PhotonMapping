#include "ONB.h"

#define ONB_EPS 0.01

void ONB::fromU(const Vector& U)
{
    Vector n(1.0, 0.0, 0.0);
    Vector m(0.0, 1.0, 0.0);
    u = U.normalize();
    v = u.cross(n);
    if (v.length() < ONB_EPS) {
        v = u.cross(m);
    }
    w = u.cross(v);
}

void ONB::fromV(const Vector& V)
{
    Vector n(1.0, 0.0, 0.0);
    Vector m(0.0, 1.0, 0.0);
    v = V.normalize();
    u = v.cross(n);
    if (u.square() < ONB_EPS) {
        u = v.cross(m);
    }
    w = u.cross(v);
}

void ONB::fromW(const Vector& W)
{
    Vector n(1.0, 0.0, 0.0);
    Vector m(0.0, 1.0, 0.0);
    w = W.normalize();
    u = w.cross(n);
    if (u.length() < ONB_EPS) {
        u = w.cross(m);
    }
    v = w.cross(u);
}

void ONB::fromUV(const Vector& U, const Vector& V)
{
    u = U.normalize();
    w = U.cross(V).normalize();
    v = w.cross(u);
}

void ONB::fromVU(const Vector& V, const Vector& U)
{
    v = V.normalize();
    w = U.cross(V).normalize();
    u = v.cross(w);
}

void ONB::fromUW(const Vector& U, const Vector& W)
{ 
    u = U.normalize();
    v = W.cross(U).normalize();
    w = u.cross(v);
}

void ONB::fromWU(const Vector& W, const Vector& U)
{
    w = W.normalize();
    v = W.cross(U).normalize();
    u = v.cross(w);
}

void ONB::fromVW(const Vector& V, const Vector& W)
{
    v = V.normalize();
    u = V.cross(W).normalize();
    w = u.cross(v);
}

void ONB::fromWV(const Vector& W, const Vector& V)
{
    w = W.normalize();
    u = V.cross(W).normalize();
    v = w.cross(u);
}

std::ostream& operator << (std::ostream& os, const ONB& onb)
{
    os << onb.w << "," << onb.v << "," << onb.w << std::endl;
    return os;
}

