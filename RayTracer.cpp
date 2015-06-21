#include "RayTracer.h"
#include "util/common.h"
#include "util/Parser.h"
#include "util/shape/Plane.h"
#include "util/shape/Sphere.h"
#include "util/shape/Triangle.h"
#include "util/shape/DSphere.h"
#include "util/shape/PointLight.h"
#include "util/shape/Quad.h"

#include <stdlib.h>

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
            Parser::checkObject(pair.second, "lights");
            for (const auto& l : pair.second.getObject()) {
                std::shared_ptr<Light> ptr;
                if (l.first == "point") {
                    ptr = PointLight::parse(l.second);
                } else if (l.first == "quad") {
                    ptr = Quad::parse(l.second);
                } else {
                    std::cerr << "Parse error: unrecognized light type \"" << l.first << "\".\n";
                    exit(1);
                }
                ptr->setMaterial(materials[Parser::getMaterial(l.second, l.first)]);
                _scene->lights.push_back(ptr);
            }
        } else if (pair.first == "camera") {
            _camera = Camera::parse(pair.second);
        } else if (pair.first == "objects") {
            Parser::checkObject(pair.second, "objects");
            for (const auto& o : pair.second.getObject()) {
                std::shared_ptr<Shape> ptr;
                if (o.first == "plane") {
                    ptr = Plane::parse(o.second);
                } else if (o.first == "sphere") {
                    ptr = Sphere::parse(o.second);
                } else if (o.first == "triangle") {
                    ptr = Triangle::parse(o.second);
                } else if (o.first == "box") {

                } else {
                    std::cerr << "Parse error: unrecognized object type \"" << o.first << "\".\n";
                    exit(1);
                }
                ptr->setMaterial(materials[Parser::getMaterial(o.second, o.first)]);
                _scene->objects.push_back(ptr);
            }
        } else if (pair.first == "global") {
            Parser::checkObject(pair.second, "global");
            for (const auto& s : pair.second.getObject()) {
                if (s.first == "num_global_photons") {
                    // NUM_GLOBAL_PHOTONS = num_global_photons;
                }
            }
        } else {
            std::cerr << "Parse error: unrecognized symbol \"" << pair.first << "\".\n";
            exit(1);
        }
    }
}

void RayTracer::buildGlobalMap()
{
    double sum = 0;
    for (const auto& light : _scene->lights) {
        sum += light->area() * light->emittance;
    }

    for (const auto& light : _scene->lights) {
        int n = light->area() * light->emittance * _num_global_photons / sum;
        for (int i = 0; i < n; ++i) {
            globalBounce(light->randomRay(), light->color);
        }
    }
}

void RayTracer::globalBounce(const Ray& ray, RGB& power)
{
    Vector dir = ray.d;
    bool inside = ray.inside; // inside object?
    TraceRecord record = _scene->intersect(ray); // trace this ray
    int bounces = 0;

    while (record.hit) {
        if (inside) {
            record.n = -record.n;
        }

        double absorvance = record.obj->absorvance;
        double roughness = record.obj->roughness;
        double reflectance;
        if (inside) {
            reflectance = this->reflectance(dir, record.n, 
                record.obj->index_of_refraction, _env->index_of_refraction);
        } else {
            reflectance = this->reflectance(dir, record.n,
                _env->index_of_refraction, record.obj->index_of_refraction);
        }
        reflectance *= (1 - absorvance);
        double refractance = (1 - absorvance) * (1 - reflectance);

        power.min(record.color);
        storePhoton(GLOBAL_MAP, power, record.v, dir);

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
        } else { // refract
            if (inside) {
                dir = refract(dir, record.n, record.obj->index_of_refraction, 
                    _env->index_of_refraction);
            } else {
                dir = refract(dir, record.n, _env->index_of_refraction, 
                    record.obj->index_of_refraction);
            }

            // reverse
            inside = !inside;
        }

        bounces++;
        if (bounces >= _max_photon_bounce) { // reach maximun bounce: stop
            break;
        }

        // else: continue bouncing
        record = _scene->intersect(Ray(record.v, dir, inside));
    }
}

void RayTracer::buildCausticsMap()
{

}

void RayTracer::renderMap()
{
// for (int i = 0; i < _width; i++) {
//     for (int j = 0; j < _height; j++) {
//         Vector dir((float)i - (float)_width/2.0f, (float)j - (float)_height/2.0f, -200.0f);
//         dir = (dir - eye).normalize();

//         TraceRecord record = _scene->intersect(Ray(eye, dir));
//         if (record.hit) {
//             RGB color = lookUpMap(CAUSTICS_MAP, record.v, 1.0, record.n);
//             if (!color.isBlack()) {
//                 _img->set(i, j, RGB(1.0, 1.0, 0.0));
//                 continue;
//             }
//             color = lookUpMap(GLOBAL_MAP, record.v, 2.0, record.n);
//             std::cout << color << std::endl;
//             if (!color.isBlack()) {
//                 _img->set(i, j, RGB(1.0, 1.0, 1.0));
//             }
//         }
//     }
// }
}

void RayTracer::render()
{
    for (int i = 0; i < _width; i++) {
    std::cout << "\r                                \r"; /* 30 spaces to cleanup the line and assure cursor is on last_char_pos + 1 */
        std::cout << "Rendering (progress: " << (i + 1) * 100.0 / _width << "%)";
        std::cout.flush();

        for (int j = 0; j < _height; j++) {
            _img->set(i, j, pixelColor(_camera->rayAt(i, j), 0));
        }
    }

    // std::cout << "Output to file " << output << ".\n";
    _img->WriteTga("test.tga", true);
}

void RayTracer::distributionRender()
{
    for (int i = 0; i < _width; i++) {
        for (int j = 0; j < _height; j++) {
        // Vector dir((float)i - (float)_width/2.0f, (float)j - (float)_height/2.0f, -200.0f);
        // dir = (dir - eye).normalize();

        // HitRecord output;
        // if (_scene->_objects[6]->hit(Ray(eye, dir), 0, 1000000, 0, output)) {
// std::cout << eye + output.t * dir << ", " << output.n << ", " << output.t << std::endl;
        // }


        // _img->set(i, j, pixelColor(Ray(eye, dir), 0));
        }
    }
}

RGB RayTracer::pixelColor(const Ray& ray, int depth)
{
    TraceRecord record = _scene->intersect(ray);

    if (depth >= _max_tracing_depth || !record.hit) {
        return RGB(); // black
    }

    if (ray.inside) {
        record.n = -record.n;
    }

    double emittance = record.obj->emittance;
    double roughness = record.obj->roughness;
    double reflectance;
    if (ray.inside) {
        reflectance = this->reflectance(ray.d, record.n, 
            record.obj->index_of_refraction, _env->index_of_refraction);
    } else {
        reflectance = this->reflectance(ray.d, record.n,
            _env->index_of_refraction, record.obj->index_of_refraction);
    }
    reflectance *= (1 - emittance);
    double refractance = 1 - reflectance - emittance;

    // reflectance + refractance + emittance = 1

    RGB color, reflect_color, refract_color, emit_color;

    if (reflectance > 0) { // reflect
        reflect_color = (1 - roughness) * pixelColor(Ray(record.v, reflect(ray.d, record.n)), depth + 1)
        + roughness * lookUpMap(GLOBAL_MAP, record.v, 100, record.n);
    }

    if (refractance > 0) { // refract
        if (ray.inside) {  // refract from object into air
            refract_color = pixelColor(
                Ray(
                    record.v, 
                    refract(ray.d, record.n, record.obj->index_of_refraction, 
                        _env->index_of_refraction),
                    false
                ), depth + 1
            ) * record.obj->color * refractance;
        } else {  // refract from air into object
            refract_color = pixelColor(
                Ray(
                    record.v, 
                    refract(ray.d, record.n, _env->index_of_refraction, 
                        record.obj->index_of_refraction),
                    true
                ), depth + 1
            ) * RGB(1, 1, 1) * refractance;
        }
    }

    if (emittance > 0) { // emit
        emit_color = emittance * record.obj->color;
    }

// std::cout << reflect_color << ", " << diffuse_color << ", " << refract_color << std::endl;

    color = emit_color + reflect_color + refract_color;
    color.scale();
    return color;
}

void RayTracer::storePhoton(int type, const RGB& power, const Vector& pos, const Vector& dir)
{
    switch (type) {
        case GLOBAL_MAP:
        _global_map->store(power, pos, dir);
        break;
        case CAUSTICS_MAP:
        _caustics_map->store(power, pos, dir);
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
        return _global_map->irradianceEstimate(center, radius, normal);
    case CAUSTICS_MAP:
        return _caustics_map->irradianceEstimate(center, radius, normal);
    default:
        fprintf(stderr, "Invalid map type: %d\n", type);
        exit(1);
    }
}

// helper functions
double RayTracer::reflectance(const Vector &dir, const Vector &normal, 
    double from, double to)
{
    /*
     * Reflectance according to http://www.bramz.net/data/writings/reflection_transmission.pdf
     */

    if (to == std::numeric_limits<double>::infinity())
        return 1.0;

    double n = from / to;
    double cosI = -(normal.dot(dir));
    double sinT2 = n * n * (1.0 - cosI * cosI);

    if (sinT2 > 1.0) /* inside TIR */
        return 1.0;

    double cosT = sqrt(1.0 - sinT2);
    double rOrth = (from * cosI - to * cosT) / (from * cosI + to * cosT);
    double rPar = (to * cosI - from * cosT) / (to * cosI + from * cosT);

    return (rOrth * rOrth + rPar * rPar) / 2.0;
}

Vector RayTracer::reflect(const Vector& incidence, const Vector& normal)
{
    return (incidence - normal * incidence.dot(normal) * 2).normalize();
}

Vector RayTracer::refract(const Vector& incidence, const Vector& normal, 
    double from, double to)
{
    double a = from / to;
    double cos1 = normal.dot(incidence) * normal.dot(incidence);
    double cos2 = sqrt(a * cos1 - a + 1.0);
    return (incidence * a + normal * (normal.dot(incidence) * a - cos2))
        .normalize();
}

Vector RayTracer::diffuse(const Vector& normal, double roughness)
{
    return normal.noise(roughness);
}
