#ifndef UTIL_KDT_H_
#define UTIL_KDT_H_

// A adaptor of libkdtree++.
// Use this to make faster range search.

#include "../third-party/kdtree++/kdtree.hpp"
#include "Vector.h"
#include "RGB.h"

#include <vector>

struct Photon
{
    RGB p;    // power
    Vector i; // incidence
    Vector l; // location
};

struct Node
{
    typedef double value_type;
    value_type xyz[3];
    Photon photon;

    double operator[] (size_t n) const
    {
        return xyz[n];
    }
};

class KDT
{
public:
    // build KD Tree
    void build(std::vector<Photon>& photons);

    // search points within sphere
    void sphereSearch(std::vector<Photon>& photons, 
        const Vector& center, double radius);

private:
    KDTree::KDTree<3, Node> _tree;
};

#endif // UTIL_KD_TREE_H_