#include "RayTracer.h"
#include "util/common.h"
#include "util/Parser.h"
#include "util/shape/Plane.h"
#include "util/shape/Sphere.h"
#include "util/shape/Triangle.h"
#include "util/shape/DSphere.h"
#include "util/shape/PointLight.h"
#include "util/shape/Quad.h"
#include "util/shape/Box.h"
#include "util/Sample.h"

#include <stdlib.h>
#include <omp.h>

void RayTracer::setup(const std::string& file)
{
    std::cout << "--> Loading scene file: " << file << "\n";

    JsonBox::Object obj = Parser::loadAndCheck(file);
    std::map<std::string, Material> materials;

    // 1. materials
    if (obj.find("materials") == obj.end()) {
        std::cerr << "Parse error: missing section \"materials\".\n";
        exit(1);
    } else {
        Parser::checkArray(obj["materials"], "materials");
        int i = 0;
        for (const auto& v : obj["materials"].getArray()) {
            char cls[16];
            sprintf(cls, "materials[%d]", i++);

            Parser::checkObject(v, cls);
            JsonBox::Object m = v.getObject();
            Parser::checkParam(m, cls, "name", Parser::STRING);
            materials[Parser::asString(m["name"])] = Material::parse(m, cls);
        }
    }

    // 2. lights
    if (obj.find("lights") == obj.end()) {
        std::cerr << "Parse error: missing section \"lights\".\n";
        exit(1);
    } else {
        Parser::checkArray(obj["lights"], "lights");
        int i = 0;
        for (const auto& v : obj["lights"].getArray()) {
            char cls[16];
            sprintf(cls, "lights[%d]", i++);

            Parser::checkObject(v, cls);
            JsonBox::Object l = v.getObject();
            Parser::checkParam(l, cls, "type", Parser::STRING);
            std::string type = Parser::asString(l["type"]);

            std::shared_ptr<Light> ptr;
            if (type == "point") {
                ptr = PointLight::parse(l, cls);
            } else if (type == "quad") {
                ptr = Quad::parse(l, cls);
            } else if (type == "box") {
                ptr = Box::parse(l, cls);
            } else {
                std::cerr << "Parse error: unrecognized light type \"" << type << "\".\n";
                exit(1);
            }
            std::string material_name = Parser::getMaterial(l, cls);
            if (materials.find(material_name) == materials.end()) {
                std::cerr << "Parse error: no such material \"" << material_name << "\".\n";
                exit(1);
            }
            ptr->setMaterial(materials[material_name]);
            _scene->lights.push_back(ptr);
            _scene->objects.push_back((std::shared_ptr<Shape>)ptr);
        }
    }

    // 3. cameras
    if (obj.find("cameras") == obj.end()) {
        std::cerr << "Parse error: missing section \"cameras\".\n";
        exit(1);
    } else {
        Parser::checkArray(obj["cameras"], "cameras");
        int i = 0;
        for (const auto& v : obj["cameras"].getArray()) {
            char cls[16];
            sprintf(cls, "cameras[%d]", i++);

            Parser::checkObject(v, cls);
            JsonBox::Object c = v.getObject();

            _cameras.push_back(Camera::parse(c));
        }
    }

    // 4. objects
    if (obj.find("objects") == obj.end()) {
        std::cerr << "Parse error: missing section \"objects\".\n";
        exit(1);
    } else {
        Parser::checkArray(obj["objects"], "objects");
        int i = 0;
        for (const auto& v : obj["objects"].getArray()) {
            char cls[16];
            sprintf(cls, "objects[%d]", i++);

            Parser::checkObject(v, cls);
            JsonBox::Object o = v.getObject();
            Parser::checkParam(o, cls, "type", Parser::STRING);
            std::string type = Parser::asString(o["type"]);

            std::shared_ptr<Shape> ptr;
            if (type == "plane") {
                ptr = Plane::parse(o, cls);
            } else if (type == "sphere") {
                ptr = Sphere::parse(o, cls);
            } else if (type == "triangle") {
                ptr = Triangle::parse(o, cls);
            } else {
                std::cerr << "Parse error: unrecognized object type \"" << type << "\".\n";
                exit(1);
            }
            std::string material_name = Parser::getMaterial(o, cls);
            if (materials.find(material_name) == materials.end()) {
                std::cerr << "Parse error: no such material \"" << material_name << "\".\n";
                exit(1);
            }
            ptr->setMaterial(materials[material_name]);
            _scene->objects.push_back(ptr);
        }
    }

    // 5. global [optional]
    if (obj.find("global") == obj.end()) {
        std::cerr << "Parse warning: missing section \"global\": loading default value.\n";
    } else {
        parseParams(obj["global"]);
    }

    // initialize
    _global_map = std::unique_ptr<KDT>(new KDT);
    _caustics_map = std::unique_ptr<KDT>(new KDT);
    _env = std::unique_ptr<Material>(new Material(0, 0, 0, 1, RGB(1, 1, 1)));

    // summary
    printf("--> Settings\n");
    printf("\tnum global photons: %d\n", _num_global_photons);
    printf("\tnum caustics photons: %d\n", _num_caustics_photons);
    printf("\tmax photon bounce: %d\n", _max_photon_bounce);
    printf("\tmax tracing depth: %d\n", _max_tracing_depth);
    printf("\tgathering radius: %lf\n", _gathering_radius);
    printf("\texposure: %lf\n", _exposure);

    printf("\tmaterials: %lu\n", materials.size());
    printf("\tlights: %lu\n", _scene->lights.size());
    printf("\tobjects: %lu\n", _scene->objects.size());

    // _samples_per_pixel = 225;
    _samples_per_pixel = 100;
}

void RayTracer::parseParams(const JsonBox::Value& val)
{
    std::string CLS = "global";
    Parser::checkObject(val, CLS);
    
    JsonBox::Object obj = val.getObject();
    Parser::checkOption(obj, CLS, "num_global_photons", Parser::INTEGER, "10000");
    Parser::checkOption(obj, CLS, "num_caustics_photons", Parser::INTEGER, "10000");
    Parser::checkOption(obj, CLS, "max_photon_bounce", Parser::INTEGER, "10");
    Parser::checkOption(obj, CLS, "max_tracing_depth", Parser::INTEGER, "10");
    Parser::checkOption(obj, CLS, "gathering_radius", Parser::NUMBER, "2.0");
    Parser::checkOption(obj, CLS, "exposure", Parser::NUMBER, "[0, 0, 0]");

    if (obj.find("num_global_photons") != obj.end()) {
        _num_global_photons = Parser::asInteger(obj["num_global_photons"]);
    }
    if (obj.find("num_caustics_photons") != obj.end()) {
        _num_caustics_photons = Parser::asInteger(obj["num_caustics_photons"]);
    }
    if (obj.find("max_photon_bounce") != obj.end()) {
        _max_photon_bounce = Parser::asInteger(obj["max_photon_bounce"]);
    }
    if (obj.find("max_tracing_depth") != obj.end()) {
        _max_tracing_depth = Parser::asInteger(obj["max_tracing_depth"]);
    }
    if (obj.find("gathering_radius") != obj.end()) {
        _gathering_radius = Parser::asNumber(obj["gathering_radius"]);
    }
    if (obj.find("exposure") != obj.end()) {
        _exposure = Parser::asNumber(obj["exposure"]);
    }
}

void RayTracer::buildGlobalMap()
{
    printf("--> Building photon map\n");

    double sum = 0;
    for (const auto& light : _scene->lights) {
        sum += light->area() * light->emittance;
    }

    for (const auto& light : _scene->lights) {
        int n = light->area() * light->emittance * _num_global_photons / sum;
        for (int i = 0; i < n; ++i) {
            Photon photon = light->randomPhoton();
            globalBounce(photon, 0);
        }
    }

    _global_map->balance();
    printf("\t%d global photon emitted.\n", _global_map->size());
}

void RayTracer::globalBounce(Photon& photon, int bounces)
{
    if (bounces >= _max_photon_bounce || photon.power.isBlack()) {
        return;
    }

    TraceRecord record = _scene->intersect(photon.ray);

    if (!record.hit) {
        return;
    }


    if (photon.ray.inside) {
        record.n = -record.n;
    }
    photon.ray.o = record.v;

    double absorvance = record.obj->absorvance;
    double roughness = record.obj->roughness;
    double reflectance;
    if (photon.ray.inside) {
        reflectance = getReflectance(photon.ray.d, record.n, 
            record.obj->index_of_refraction, _env->index_of_refraction);
    } else {
        reflectance = getReflectance(photon.ray.d, record.n,
            _env->index_of_refraction, record.obj->index_of_refraction);
    }
    reflectance *= (1 - absorvance);
    double refractance = (1 - absorvance) * (1 - reflectance);

    double random = drand48(); // russian roulette
    if (random < absorvance) { // absorb
        return;
    }

    if (random < absorvance + reflectance) { // reflect
        photon.power.min(record.color);

        if ((random - absorvance) / reflectance < roughness) { // diffuse reflect
            photon.ray.d = record.n;
            storePhoton(GLOBAL_MAP, photon);
            photon.ray.d = diffuse(record.n, roughness);
        } else { // specular reflect
            photon.ray.d = reflect(photon.ray.d, record.n);
        }
    } else { // refract
        if (photon.ray.inside) {
            photon.ray.d = refract(photon.ray.d, record.n, record.obj->index_of_refraction, 
                _env->index_of_refraction);
        } else {
            photon.ray.d = refract(photon.ray.d, record.n, _env->index_of_refraction, 
                record.obj->index_of_refraction);
        }

        // reverse
        photon.ray.inside = !photon.ray.inside;
    }

    // continue
    globalBounce(photon, bounces + 1);
}

void RayTracer::buildCausticsMap()
{
    double sum = 0;
    for (const auto& light : _scene->lights) {
        sum += light->area() * light->emittance;
    }

    for (const auto& light : _scene->lights) {
        int n = light->area() * light->emittance * _num_caustics_photons / sum;
        for (int i = 0; i < n; ++i) {
            Photon photon = light->randomPhoton();
            causticsBounce(photon, 0);
        }
    }

    _caustics_map->balance();
    printf("%d caustics photon emitted.\n", _caustics_map->size());
}

void RayTracer::causticsBounce(Photon& photon, int bounces)
{
    if (bounces >= _max_photon_bounce || photon.power.isBlack()) {
        return;
    }

    TraceRecord record = _scene->intersect(photon.ray);

    if (!record.hit) {
        return;
    }

    if (photon.ray.inside) {
        record.n = -record.n;
    }
    photon.ray.o = record.v;

    double absorvance = record.obj->absorvance;
    double roughness = record.obj->roughness;
    double reflectance;
    if (photon.ray.inside) {
        reflectance = getReflectance(photon.ray.d, record.n, 
            record.obj->index_of_refraction, _env->index_of_refraction);
    } else {
        reflectance = getReflectance(photon.ray.d, record.n,
            _env->index_of_refraction, record.obj->index_of_refraction);
    }
    reflectance *= (1 - absorvance);
    double refractance = (1 - absorvance) * (1 - reflectance);

    double random = drand48(); // russian roulette
    if (random < absorvance) { // absorb
        return;
    }

    if (random < absorvance + reflectance) { // reflect
        photon.power.min(record.color);

        if ((random - absorvance) / reflectance < roughness) { // diffuse reflect
            photon.ray.d = record.n;
            if (photon.caustics) {
                storePhoton(CAUSTICS_MAP, photon);
            }
            // photon.ray.d = diffuse(record.n, roughness);
            // return;
        } else { // specular reflect
            photon.ray.d = reflect(photon.ray.d, record.n);
        }
    } else { // refract
        if (photon.ray.inside) {
            photon.ray.d = refract(photon.ray.d, record.n, record.obj->index_of_refraction, 
                _env->index_of_refraction);
        } else {
            photon.ray.d = refract(photon.ray.d, record.n, _env->index_of_refraction, 
                record.obj->index_of_refraction);
        }

        // reverse
        photon.ray.inside = !photon.ray.inside;

        // flag
        photon.caustics = true;
    }

    // continue
    causticsBounce(photon, bounces + 1);
}

void RayTracer::renderMap()
{
    int width = _cameras[0]->width;
    int height = _cameras[0]->height;
    _img = std::unique_ptr<Image>(new Image(width, height));

    int cnt = 0;

        #pragma omp parallel for
        for (int i = 0; i < width * height; i++) {
            Ray ray = _cameras[0]->rayAt(i % width, i / width);
            TraceRecord record = _scene->intersect(ray);
            if (ray.inside) {
                record.n = -record.n;
            }
            if (record.hit) {
                RGB color = lookUpMap(GLOBAL_MAP, record.v, _gathering_radius, record.n);
                _img->set(i, color);
            }

            if (!(i % width)) {
                #pragma omp critical
                {
                    std::cout << "\r                                \r";  //30 spaces to cleanup the line and assure cursor is on last_char_pos + 1 
                    std::cout << "Rendering (progress: " << (cnt += width) * 100.0 / (width * height) << "%)";
                    std::cout.flush();
                }
            }
        }
        

        std::cout << std::endl;
        _img->dumpPPM("test.ppm");
}

void RayTracer::renderMap2()
{
    int width = _cameras[0]->width;
    int height = _cameras[0]->height;
    _img = std::unique_ptr<Image>(new Image(width, height));

    int cnt = 0;

        #pragma omp parallel for
        for (int i = 0; i < width * height; i++) {
            Ray ray = _cameras[0]->rayAt(i % width, i / width);
            TraceRecord record = _scene->intersect(ray);
            if (ray.inside) {
                record.n = -record.n;
            }
            if (record.hit) {
                RGB color = record.color;
                _img->set(i, color);
            }

            if (!(i % width)) {
                #pragma omp critical
                {
                    std::cout << "\r                                \r";  //30 spaces to cleanup the line and assure cursor is on last_char_pos + 1 
                    std::cout << "Rendering (progress: " << (cnt += width) * 100.0 / (width * height) << "%)";
                    std::cout.flush();
                }
            }
        }
        

        std::cout << std::endl;
        _img->dumpPPM("test.ppm");
}

void RayTracer::fastRender()
{
    int curr = 0;
    int total = _cameras.size();
    for (const auto& c : _cameras) {
        std::cout << "--> Rendering: " << c->output << " (" << ++curr << " / " << total << ")\n";

        int width = c->width;
        int height = c->height;
        Image img(width, height);

        int cnt = 0;

        #pragma omp parallel for
        for (int i = 0; i < width * height; i++) {
            img.set(i, pixelColor(c->rayAt(i % width, i / width)));

            if (!(i % width)) {
                #pragma omp critical
                {
                    std::cout << "\r                             \r";
                    std::cout << "\tprogress: " << (cnt += width) * 100.0 / (width * height) << "%";
                    std::cout.flush();
                }
            }
        }
        std::cout << std::endl;

        img.dumpPPM("test.ppm");
    }
}

void RayTracer::render()
{
    for (const auto& c : _cameras) {
        int width = c->width;
        int height = c->height;
        Image img(width, height);

        int cnt = 0;

        int n = 10;
        std::vector<Point> samples;
        double inv = 1.0 / n;
        double offset = inv / 2.0;
        for (int i = 0; i < n; ++i) {
            for (int j = 0; j < n; ++j) {
                samples.push_back(Point(offset + inv * i - 0.5, offset + inv * j - 0.5));
            }
        }

        #pragma omp parallel for
        for (int i = 0; i < width * height; i++) {
            // render each pixel several times
            RGB color;
            for (const auto& p : samples) {
                color += pixelColor(c->rayAt(i % width + p.x, i / width + p.y));
            }

            // set pixel with average color
            img.set(i, color / (double)(n * n));

            if (!(i % width)) {
                #pragma omp critical
                {
                    std::cout << "\r                             \r";
                    std::cout << "\tprogress: " << (cnt += width) * 100.0 / (width * height) << "%";
                    std::cout.flush();
                }
            }
        }
        std::cout << std::endl;

        img.dumpPPM("test.ppm");
    }
}

RGB RayTracer::pixelColor(const Ray& ray, int depth, double relevance)
{
    TraceRecord record = _scene->intersect(ray);

    if (depth >= _max_tracing_depth || relevance < EPS || !record.hit) {
        return RGB(); // black
    }

    if (ray.inside) {
        record.n = -record.n;
    }

    double emittance = record.obj->emittance;
    double roughness = record.obj->roughness;
    double reflectance;
    if (ray.inside) {
        reflectance = getReflectance(ray.d, record.n, 
            record.obj->index_of_refraction, _env->index_of_refraction);
    } else {
        reflectance = getReflectance(ray.d, record.n,
            _env->index_of_refraction, record.obj->index_of_refraction);
    }
    reflectance = (1 - emittance) * reflectance;
    double refractance = 1 - reflectance - emittance;

    // reflectance + refractance + emittance = 1

    RGB color, reflect_color, refract_color, emit_color;

    if (reflectance > 0) { // reflect
        reflect_color = roughness * lookUpMap(GLOBAL_MAP, record.v, _gathering_radius, record.n) 
            * _exposure // diffuse
            + (1 - roughness)
            * pixelColor(Ray(record.v, reflect(ray.d, record.n)), depth + 1, 
                relevance * reflectance * (1 - roughness)) // specular
        ;
    }

    if (refractance > 0) { // refract
        Vector dir;
        if (ray.inside) {  // refract from object into air
            dir = refract(ray.d, record.n, record.obj->index_of_refraction, 
                _env->index_of_refraction);
            refract_color = record.color;
        } else {  // refract from air into object
            dir = refract(ray.d, record.n, _env->index_of_refraction,
                record.obj->index_of_refraction);
            refract_color = RGB(1, 1, 1);
        }

        refract_color = refract_color * pixelColor(Ray(record.v, dir, !ray.inside),
            depth + 1, relevance * refractance);
    }

    if (emittance > 0) { // emit
        emit_color = record.obj->color;
    }

    color = emittance * emit_color + reflectance * reflect_color + refractance * refract_color;
        // + lookUpMap(CAUSTICS_MAP, record.v, _gathering_radius, record.n);
    color.scale();

    return color;
}

void RayTracer::storePhoton(int type, const Photon& photon)
{
    switch (type) {
    case GLOBAL_MAP:
        _global_map->store(photon);
        break;
    case CAUSTICS_MAP:
        _caustics_map->store(photon);
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
// refer to:
//     http://www.bramz.net/data/writings/reflection_transmission.pdf
double RayTracer::getReflectance(const Vector &dir, const Vector &normal, 
    double from, double to)
{
    if (to == std::numeric_limits<double>::infinity())
        return 1.0;

    double n = from / to;
    double cos_i = -(normal.dot(dir));
    double sin_t = n * n * (1.0 - cos_i * cos_i);

    if (sin_t > 1.0) {
        return 1.0;
    }

    double cos_t = sqrt(1.0 - sin_t);
    double r1 = (from * cos_i - to * cos_t) / (from * cos_i + to * cos_t);
    double r2 = (to * cos_i - from * cos_t) / (to * cos_i + from * cos_t);

    return (r1 * r1 + r2 * r2) / 2.0;
}

Vector RayTracer::reflect(const Vector& incidence, const Vector& normal)
{
    return (incidence - normal * incidence.dot(normal) * 2).normalize();
}

Vector RayTracer::refract(const Vector& incidence, const Vector& normal, 
    double from, double to)
{
    double n = from / to;
    double cos_i = -(normal.dot(incidence));
    double sin_t = n * n * (1.0 - cos_i * cos_i);

    assert(sin_t <= 1.0);

    double cos_t = sqrt(1.0 - sin_t);

    return (incidence * n + normal * (n * cos_i - cos_t)).normalize();
}

Vector RayTracer::diffuse(const Vector& normal, double roughness)
{
    return normal.noise(roughness);
}
