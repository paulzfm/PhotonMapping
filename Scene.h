#ifndef SCENE_H_
#define SCENE_H_

#include "util/Ray.h"
#include "util/shape/Shape.h"
#include "util/shape/Light.h"

#include <string>
#include <vector>

struct TraceRecord
{
    bool hit; // hit?
    double t; // parameter t
    Vector n; // normal vector
    Vector v; // hitting point
    std::shared_ptr<Shape> obj; // object
    RGB color;

    TraceRecord() { hit = false; }
};

class Scene
{
public:
    // add shape to scene
    void addShape(std::shared_ptr<Shape> shape);

    // add light to scene
    void addLight(std::shared_ptr<Light> light);

    // intersect?
    TraceRecord intersect(const Ray& ray);

    void print();

    std::vector< std::shared_ptr<Shape> > objects;
    std::vector< std::shared_ptr<Light> > lights;
};

#endif // SCENE_H_