#ifndef SCENE_H_
#define SCENE_H_

#include "util/Ray.h"
#include "util/shape/Shape.h"

#include <string>
#include <vector>

struct TraceRecord
{
    bool hit; // hit?
    double t; // parameter t
    Vector n; // normal vector
    Vector v; // hitting point
    std::shared_ptr<Shape> obj; // object
    int idx;  // object index
    RGB color;

    TraceRecord() { hit = false; }
};

class Scene
{
public:
    // add shape to scene
    void addShape(std::shared_ptr<Shape> shape);

    // intersect?
    TraceRecord intersect(const Ray& ray);

private:
    std::vector< std::shared_ptr<Shape> > _objects;
};

#endif // SCENE_H_