#ifndef ONB_H_
#define ONB_H_

#include "Vector.h"
#include <iostream>

class ONB
{
public:
    ONB() {}
    ONB(const Vector& a, const Vector& b, const Vector& c)
        : u(a), v(b), w(c) {}

    void fromU(const Vector& U);
    void fromV(const Vector& V);
    void fromW(const Vector& W);
    
    // Calculate the ONB from two vectors
    // The first one is the Fixed vector (it is just normalized)
    // The second is normalized and its direction can be adjusted
    void fromUV(const Vector& U, const Vector& V);
    void fromVU(const Vector& V, const Vector& U);
    void fromUW(const Vector& U, const Vector& W);
    void fromWU(const Vector& W, const Vector& U);
    void fromVW(const Vector& V, const Vector& W);
    void fromWV(const Vector& W, const Vector& V);

    friend std::ostream& operator << (std::ostream& os, const ONB& onb);

    Vector u;
    Vector v;
    Vector w;
};

#endif // ONB_H_