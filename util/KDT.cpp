#include "KDT.h"

void KDT::build(std::vector<Photon>& photons)
{
    for (const auto& photon : photons) {
        Node node;
        node.photon = photon;
        node.xyz[0] = photon.l.x;
        node.xyz[1] = photon.l.y;
        node.xyz[2] = photon.l.z;

        _tree.insert(node);
    }
}

void KDT::sphereSearch(std::vector<Photon>& photons, 
    const Vector& center, double radius)
{
    Node center_node;
    center_node.xyz[0] = center.x;
    center_node.xyz[1] = center.y;
    center_node.xyz[2] = center.z;

    std::vector<Node> nodes;
    _tree.find_within_range(center_node, radius, 
        std::back_insert_iterator< std::vector<Node> >(nodes));

    for (const auto& node : nodes) {
        if (Vector::distance(center, node.photon.l) < radius) {
            photons.push_back(node.photon);
        }
        // else {
            // std::cout << "out of range" << std::endl;
        // }
    }
}