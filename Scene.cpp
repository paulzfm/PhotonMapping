#include "Scene.h"

#include <iostream>
#include <assert.h>
#include <map>

TraceRecord Scene::intersect(const Ray& ray)
{
    TraceRecord res;
    double max_dis = std::numeric_limits<double>::max();

    for (int i = 0; i < objects.size(); i++) {
        HitRecord record;
        if (objects[i]->hit(ray, 0, record)) {
            Vector v = ray.o + record.t * ray.d;
            double dis = (v - ray.o).square();
            if (dis < max_dis) {
                res.idx = i;
                res.obj = objects[i];
                res.t = record.t;
                res.n = record.n;
                res.v = v;
                res.hit = true;
                res.color = record.color;
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
