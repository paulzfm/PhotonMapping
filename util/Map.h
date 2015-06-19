#ifndef UTIL_MAP_H_
#define UTIL_MAP_H_

// A adaptor of PhotonMap.

#include "../third-party/PhotonMap.h"
#include "RGB.h"
#include "Vector.h"

class Map
{
public:
    Map(int max_photon)
        : _map(max_photon) {}

    void store(const RGB& power, const Vector& pos, const Vector& dir);

    void scale(int num_of_photons);

    RGB irradianceEstimate(const Vector& pos, double radius, const Vector& normal);

    void print();

private:
    PhotonMap _map;
};

#endif // UTIL_MAP_H_