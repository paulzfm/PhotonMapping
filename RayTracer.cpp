#include "RayTracer.h"
#include "util/common.h"
#include "util/Parser.h"
#include "util/shape/Plane.h"
#include "util/shape/Sphere.h"
#include "util/shape/Triangle.h"
#include "util/shape/DSphere.h"

#include <stdlib.h>

void RayTracer::setup(const std::string& file)
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
            _camera = Camera::parse(pair.second);
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
                _scene->addShape(ptr);
            }
        } else if (pair.first == "global") {
            
        } else {
            std::cerr << "Parse error: unrecognized symbol \"" << pair.first << "\".\n";
            exit(1);
        }
    }
}

void RayTracer::buildGlobalMap(int num_of_bounces)
{
    int num = 0;
    RGB power = RGB(1.0, 1.0, 1.0) * (1.0 / NUM_GLOBAL_PHOTONS);

    while (num < NUM_GLOBAL_PHOTONS) {
        Ray ray = light.randomRay();
        Vector dir = ray.d;
        TraceRecord record = _scene->intersect(ray); // trace this ray
        int bounces = 0;

        while (record.hit && bounces <= num_of_bounces) {
            double absorvance = record.obj->absorvance;
            double roughness = record.obj->roughness;
            double reflectance = (1 - absorvance) * record.obj->reflectance;
            double refractance = (1 - absorvance) * (1 - reflectance);

            // power = RGB::filter(power, record.color);
            storePhoton(GLOBAL_MAP, power, record.v, dir);
            num++;

            double random = drand48(); // russian roulette
            if (random < absorvance) { // absorb
                break;
            }

            if (random < absorvance + reflectance) { // reflect
                if ((random - absorvance) / reflectance < roughness) { // diffuse reflect
                    dir = diffuse(record.n, roughness);
                } else { // specular reflect
                    dir = reflect(dir, record.n);
                }
            } else { // refract: travel through object
                // refract from air into object
                dir = refract(dir, record.n, record.obj->index_of_refraction, 
                    false);
                record = _scene->intersect(Ray(record.v, dir));
                if (!record.hit) {
                    break;
                }

                // refract from object into air
                dir = refract(dir, record.n, record.obj->index_of_refraction, 
                    true);
            }

            record = _scene->intersect(Ray(record.v, dir));
            bounces++;
        }
    }

    printf("%d global photons found.\n", num);

    // scale
    _global_map.scale(1);
}

void RayTracer::buildCausticsMap(int num_of_bounces)
{
    
}

void RayTracer::renderMap()
{
    for (int i = 0; i < _width; i++) {
        for (int j = 0; j < _height; j++) {
            Vector dir((float)i - (float)_width/2.0f, (float)j - (float)_height/2.0f, -200.0f);
            dir = (dir - eye).normalize();
            
            TraceRecord record = _scene->intersect(Ray(eye, dir));
            if (record.hit) {
                RGB color = lookUpMap(CAUSTICS_MAP, record.v, 1.0, record.n);
                if (!color.isBlack()) {
                    _img->set(i, j, RGB(1.0, 1.0, 0.0));
                    continue;
                }
                color = lookUpMap(GLOBAL_MAP, record.v, 2.0, record.n);
                std::cout << color << std::endl;
                if (!color.isBlack()) {
                    _img->set(i, j, RGB(1.0, 1.0, 1.0));
                }
            }
        }
    }
}

void RayTracer::render()
{
    for (int i = 0; i < _width; i++) {
        for (int j = 0; j < _height; j++) {
            Vector dir((float)i - (float)_width/2.0f, (float)j - (float)_height/2.0f, -200.0f);
            dir = (dir - eye).normalize();

            // HitRecord output;
            // if (_scene->_objects[6]->hit(Ray(eye, dir), 0, 1000000, 0, output)) {
// std::cout << eye + output.t * dir << ", " << output.n << ", " << output.t << std::endl;
            // }


            _img->set(i, j, pixelColor(Ray(eye, dir), 0));
        }
    }
}

void RayTracer::distributionRender()
{
    for (int i = 0; i < _width; i++) {
        for (int j = 0; j < _height; j++) {
            Vector dir((float)i - (float)_width/2.0f, (float)j - (float)_height/2.0f, -200.0f);
            dir = (dir - eye).normalize();

            // HitRecord output;
            // if (_scene->_objects[6]->hit(Ray(eye, dir), 0, 1000000, 0, output)) {
// std::cout << eye + output.t * dir << ", " << output.n << ", " << output.t << std::endl;
            // }


            _img->set(i, j, pixelColor(Ray(eye, dir), 0));
        }
    }
}

void RayTracer::setImgSize(int width, int height)
{
    _img = std::unique_ptr<Image>(new Image(width, height));
    _width = width;
    _height = height;
}

void RayTracer::changeCamera(std::unique_ptr<Camera> camera)
{
    _camera = std::move(camera);
}

RGB RayTracer::pixelColor(const Ray& ray, int depth)
{
    TraceRecord record = _scene->intersect(ray);
    RGB color, reflect_color, refract_color, emit_color;

    if (depth >= MAX_DEPTH || !record.hit) {
        return color; // black
    }

    double emittance = record.obj->emittance;
    double roughness = record.obj->roughness;
    double reflectance = (1 - emittance) * record.obj->reflectance;
    double refractance = 1 - reflectance - emittance;

    if (reflectance > 0) { // reflect
        reflect_color = (1 - roughness) * pixelColor(Ray(record.v, reflect(ray.d, record.n)), depth + 1)
                      + roughness * lookUpMap(GLOBAL_MAP, record.v, 100, record.n);
    }

    if (refractance > 0) { // refract
        if (ray.air) {  // refract from object into air
            refract_color = pixelColor(
                Ray(
                    record.v, 
                    refract(ray.d, record.n, record.obj->index_of_refraction, true),
                    !ray.air
                ), depth + 1
            ) * record.obj->color * refractance;
        } else {  // refract from air into object
            refract_color = pixelColor(
                Ray(
                    record.v, 
                    refract(ray.d, record.n, record.obj->index_of_refraction, false),
                    !ray.air
                ), depth + 1
            ) * RGB(1, 1, 1) * refractance;
        }
    }

    if (emittance > 0) { // emit
        emit_color = emittance * record.obj->color;
    }

    // std::cout << reflect_color << ", " << diffuse_color << ", " << refract_color << std::endl;

    color = emit_color + reflect_color ;//+ refract_color;
    color.scale();
    return color;
}

void RayTracer::storePhoton(int type, const RGB& power, const Vector& pos, const Vector& dir)
{
    switch (type) {
    case GLOBAL_MAP:
        _global_map.store(power, pos, dir);
        break;
    case CAUSTICS_MAP:
        _caustics_map.store(power, pos, dir);
        break;
    default:
        fprintf(stderr, "Invalid map type: %d\n", type);
        exit(1);
    }
}

RGB RayTracer::lookUpMap(int type, const Vector& center, double radius, 
    const Vector& normal)
{
    switch (type) {
    case GLOBAL_MAP:
        return _global_map.irradianceEstimate(center, radius, normal);
    case CAUSTICS_MAP:
        return _caustics_map.irradianceEstimate(center, radius, normal);
    default:
        fprintf(stderr, "Invalid map type: %d\n", type);
        exit(1);
    }
}

// helper functions
Vector RayTracer::reflect(const Vector& incidence, const Vector& normal)
{
    return (incidence - normal * incidence.dot(normal) * 2).normalize();
}

Vector RayTracer::refract(const Vector& incidence, const Vector& normal, 
    double index_of_refraction, bool air)
{
    double a = air ? (1.0 / index_of_refraction) : index_of_refraction;
    double cos1 = normal.dot(incidence) * normal.dot(incidence);
    double cos2 = sqrt(a * cos1 - a + 1.0);
    return (incidence * a + normal * (normal.dot(incidence) * a - cos2))
        .normalize();
}

Vector RayTracer::diffuse(const Vector& normal, double roughness)
{
    double radius = drand48() * roughness;
    double theta = drand48() * (PI * 2);
    double a = radius * cos(theta);
    double b = radius * sin(theta);

    Vector u = normal.cross(Vector(0, 1, 0));
    if (u.length() != 1)
        u = normal.cross(Vector(1, 0, 0));
    Vector v = normal.cross(u);
    
    return (normal * radius + u * a + v * b).normalize();
}
