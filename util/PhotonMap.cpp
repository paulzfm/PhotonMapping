#include "PhotonMap.h"
#include "common.h"

void PhotonMap::store(const Photon& photon)
{
    _tree.insert(photon);
    ++_size;
}

void PhotonMap::balance()
{
    _tree.optimise();
}

int PhotonMap::size() const
{
    return _size;
}

RGB PhotonMap::irradianceEstimate(const Vector& center, double radius, const Vector& normal)
{
    Photon center_node;
    center_node.ray.o = center;

    std::vector<Photon> nodes;
    _tree.find_within_range(center_node, radius, 
        std::back_insert_iterator< std::vector<Photon> >(nodes));

    double square_radius = radius * radius;
    RGB color;
    for (const auto& e : nodes) {
        double weight = e.ray.d.dot(normal);
        if (weight > 0 && (center - e.ray.o).square() < square_radius) {
            color += e.power * weight;
        }
    }

    color *= 1.0 / (PI * square_radius);
    return color;
}

