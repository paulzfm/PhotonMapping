#include "Scene.h"
#include "util/Parser.h"
#include "util/shape/Plane.h"
#include "util/shape/Sphere.h"
#include "util/shape/Triangle.h"
#include "util/shape/DSphere.h"

#include <iostream>
#include <assert.h>
#include <map>

TraceRecord Scene::intersect(const Ray& ray)
{
    TraceRecord res;
    double tmin = -999999.9;
    double tmax = 999999.9;

    for (int i = 0; i < _objects.size(); i++) {
        HitRecord record;
        if (_objects[i]->hit(ray, tmin, tmax, 0, record)) {
            if (record.t <= tmax) {
                res.idx = i;
                res.obj = _objects[i];
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

void Scene::setup(const std::string& file)
{
    JsonBox::Object obj = Parser::loadAndCheck(file);
    std::map<std::string, Material> materials;

    for (const auto& pair : obj) {
        if (pair.first == "materials") {
            Parser::checkObject(pair.second, "materials");
            for (const auto& m : pair.second.getObject()) {
                materials[m.first] = Material::parse(m.second);
            }
        } else if (pair.first == "lights") {

        } else if (pair.first == "camera") {

        } else if (pair.first == "objects") {
            Parser::checkArray(pair.second, "objects");
            std::shared_ptr<Shape> ptr;
            for (const auto& o : pair.second.getObject()) {
                if (o.first == "plane") {
                    ptr = Plane::parse(o.second);
                } else if (o.first == "sphere") {
                    ptr = Sphere::parse(o.second);
                }

                ptr->setMaterial(materials[Parser::getMaterial(o.second, o.first)]);
                _objects.push_back(ptr);
            }
        } else {
            std::cerr << "Parse error: unrecognized symbol \"" << pair.first << "\".\n";
            exit(1);
        }
    }
}