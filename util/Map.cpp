#include "Map.h"

void Map::store(const RGB& power, const Vector& pos, const Vector& dir)
{
    double __power[3] = {power.r, power.g, power.b};
    double __pos[3] = {pos.x, pos.y, pos.z};
    double __dir[3] = {dir.x, dir.y, dir.z};

    _map.store(__power, __pos, __dir);
}

void Map::scale(int num_of_photons)
{
    _map.balance();
}

int Map::size()
{
    return _map.get_stored_photons();
}

RGB Map::irradianceEstimate(const Vector& pos, double radius, const Vector& normal)
{
    double irrad[3];
    double __pos[3] = {pos.x, pos.y, pos.z};
    double __normal[3] = {normal.x, normal.y, normal.z};

    _map.irradiance_estimate(irrad, __pos, __normal, radius, (int)(3 * radius));

    return RGB(irrad[0], irrad[1], irrad[2]);
}

void Map::print()
{
    int n = _map.get_stored_photons();
    Photon* p = _map.getPhotons();

    for (int i = 0; i < n; i++) {
        printf("(%lf, %lf, %lf)\n", p[i].pos[0], p[i].pos[1], p[i].pos[2]);
    }
}