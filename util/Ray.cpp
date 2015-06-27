#include "Ray.h"

std::ostream& operator << (std::ostream& os, const Ray& r)
{
    os << r.o << "+t" << r.d;
    return os;
}