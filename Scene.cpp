#include "Scene.h"

#include <iostream>
#include <assert.h>
#include <map>

TraceRecord Scene::intersect(const Ray& ray)
{
    TraceRecord res;
    double max_dis = std::numeric_limits<double>::max();

    for (const auto& obj : objects) {
        HitRecord record;
        if (obj->hit(ray, 0, record)) {
            Vector v = ray.o + record.t * ray.d;
            double dis = (v - ray.o).square();
            if (dis < max_dis) {
                res.obj = obj;
                res.t = record.t;
                res.n = record.n;
                res.v = v;
                res.hit = true;
                res.color = obj->colorAt(v);
                max_dis = dis;
            }
        }
    }

    return res;
}

void Scene::addShape(std::shared_ptr<Shape> shape)
{
    objects.push_back(shape);
}

void Scene::addLight(std::shared_ptr<Light> light)
{
    lights.push_back(light);
}

void Scene::print()
{
}
