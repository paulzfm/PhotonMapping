#ifndef UTIL_PHOTON_MAP_H_
#define UTIL_PHOTON_MAP_H_

// A adaptor of libPhotonMapree++.
// Use this to make faster range search.

#include "../third-party/kdtree++/kdtree.hpp"
#include "Vector.h"
#include "RGB.h"
#include "Ray.h"

#include <vector>

struct Photon
{
    RGB power; // power
    Ray ray;   // ray
    bool caustics;

    Photon() : caustics(false) {}

    // postion: for PhotonMap
    typedef double value_type;
    value_type operator[] (size_t n) const 
    {
        if (n == 0) return ray.o.x;
        if (n == 1) return ray.o.y;
        if (n == 2) return ray.o.z;
        return -1;
    };
};

class PhotonMap
{
public:
    PhotonMap() { _size = 0; }

    void store(const Photon& photon);

    void balance();

    int size() const;

    RGB irradianceEstimate(const Vector& center, double radius, const Vector& normal);

private:
    KDTree::KDTree<3, Photon> _tree;
    int _size;
};

#endif // UTIL_KD_TREE_H_