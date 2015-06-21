#include "Scene.h"

#include <iostream>
#include <assert.h>
#include <map>

TraceRecord Scene::intersect(const Ray& ray)
{
    TraceRecord res;
    double tmin = -999999.9;
    double tmax = 999999.9;

    for (int i = 0; i < objects.size(); i++) {
        HitRecord record;
        if (objects[i]->hit(ray, tmin, tmax, 0, record)) {
            if (record.t <= tmax) {
                res.idx = i;
                res.obj = objects[i];
                res.t = record.t;
                res.v = ray.o + record.t * ray.d;
                res.n = record.n;
                res.hit = true;
                res.color = record.color;
                tmax = record.t;
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
