#include "RayTracer.h"
#include "util/common.h"

#include <stdlib.h>

void RayTracer::buildGlobalMap(int num_of_bounces)
{
    int num = 0;
    RGB power = RGB(1.0, 1.0, 1.0) * 100.0 * (1.0 / NUM_GLOBAL_PHOTONS);

    while (num < NUM_GLOBAL_PHOTONS) {
        Ray ray = light.randomRay();
        Vector dir = ray.d;
        TraceRecord record = _scene.intersect(ray); // trace this ray
        int bounces = 0;
        RGB color = power;

        while (record.hit && bounces <= num_of_bounces) {
            color = RGB::filter(color, record.color);

            Photon photon;
            photon.p = color * color.a;
            photon.i = ray.d;
            photon.l = record.v;
            storePhoton(GLOBAL_MAP, record.idx, photon);

            // diffuse or reflect?
            double random = drand48(); // russian roulette
            if (random < 0.3) { // diffuse
                dir = diffuse(dir);
            } else if (random >= 0.3 && random < 0.7) { // reflect
                dir = reflect(dir, record.n);
            } else { // absorb
                break;
            }

            if (color.a < 1.0) { // transparent: travel through object
                // refract from air into object
                dir = refract(dir, record.n, record.obj->index_of_refraction, 
                    false);
                record = _scene.intersect(Ray(record.v, dir));
                if (!record.hit) {
                    break;
                }

                // refract from object into air
                dir = refract(dir, record.n, record.obj->index_of_refraction, 
                    true);
            }

            record = _scene.intersect(Ray(record.v, dir));
            bounces++;
        }
        num++;
    }

    printf("%lu global photons found.\n", _global_buffer.size());

    // build KD Tree
    _global_map.build(_global_buffer);
}

void RayTracer::buildCausticsMap(int num_of_bounces)
{

}

void RayTracer::renderMap()
{
    for (int i = 0; i < _width; i++) {
        for (int j = 0; j < _height; j++) {
            Vector dir((float)i - (float)_width/2.0f, (float)j - (float)_height/2.0f, -200.0f);
            dir = dir - eye;
            dir.normalize();
            
            TraceRecord record = _scene.intersect(Ray(eye, dir));
            if (record.hit) {
                RGB color = lookUpMap(GLOBAL_MAP, record.v, 100.0, record.n, record.idx);
                _img->set(i, j, color);
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
            // if (_scene._objects[6]->hit(Ray(eye, dir), 0, 1000000, 0, output)) {
// std::cout << eye + output.t * dir << ", " << output.n << ", " << output.t << std::endl;
            // }


            _img->set(i, j, pixelColor(Ray(eye, dir), 0));
        }
    }
}

void RayTracer::setCamera(Camera& camera)
{
    // _camera(camera);
}

void RayTracer::setImage(Image* img)
{
    _img = img;
    _width = img->width();
    _height = img->height();
}

RGB RayTracer::pixelColor(const Ray& ray, int depth)
{
    TraceRecord record = _scene.intersect(ray);
    RGB color, pix_color;

    if (record.hit) {
        Vector line = (light.pos - record.v).normalize();
        double v_dot = record.n.dot(line);
        Vector ref = record.n * 2 * v_dot - line;
        double specular = pow(ref.dot(-ray.d), specular_power);
        double intensity = Ia*Ka+(Id*Kd*v_dot);

        pix_color = record.color * intensity;

        RGB g_color = lookUpMap(GLOBAL_MAP, record.v, 100.0, record.n, 
            record.idx);
        RGB c_color = lookUpMap(CAUSTICS_MAP, record.v, 100.0, record.n, 
            record.idx);

        pix_color = pix_color + g_color * Kp + Is * Ks * specular;
        color = pix_color;

        if (depth >= MAX_DEPTH) {
            color.scale();
            return color;
        }

        if (record.obj->reflective > 0.0) { // reflect
            RGB reflect_color = pixelColor(
                Ray(record.v, reflect(ray.d, record.n)), depth + 1
            );
            color = pix_color * (1.0 - record.obj->reflective) + 
                reflect_color * record.obj->reflective +
                Is * Ks * specular;
        }

        if (pix_color.a < 1.0) { // transparent: refract
            // refract from air into object
            Vector refract_in = refract(ray.d, record.n, 
                record.obj->index_of_refraction, false);
            TraceRecord record1 = _scene.intersect(Ray(record.v, refract_in));
            if (!record1.hit) {
                color.scale();
                return color;
            }

            // refract from object into air
            Vector refract_out = refract(refract_in, record1.n, 
                record1.obj->index_of_refraction, true);
            RGB refract_color = pixelColor(
                Ray(record1.v, refract_out), depth + 1
            );
            color = pix_color * pix_color.a + 
                refract_color * (1.0 - pix_color.a) +
                Is * Ks * specular;
        }
    }

    color.scale();
    return color;
}

void RayTracer::storePhoton(int type, int idx, const Photon& photon)
{
    switch (type) {
    case GLOBAL_MAP:
        _global_buffer.push_back(photon);
        break;
    case CAUSTICS_MAP:
        _caustics_buffer.push_back(photon);
        break;
    default:
        fprintf(stderr, "Invalid map type: %d\n", type);
        exit(1);
    }
}

RGB RayTracer::lookUpMap(int type, const Vector& center, double radius, 
    const Vector& normal, int idx)
{
    std::vector<Photon> photons;
    switch (type) {
    case GLOBAL_MAP:
        _global_map.sphereSearch(photons, center, radius);
        break;
    case CAUSTICS_MAP:
        _caustics_map.sphereSearch(photons, center, radius);
        break;
    default:
        fprintf(stderr, "Invalid map type: %d\n", type);
        exit(1);
    }

    RGB color;
    for (const auto& photon : photons) {
        double weight = MAX(0.0, -normal.dot(photon.i));
        double distance = (center - photon.l).length();
        weight *= (radius - distance) / radius;
        color += weight * photon.p;
    }

    return color;
}

// helper functions
Vector RayTracer::reflect(const Vector& incidence, const Vector& normal)
{
    return (incidence - normal * incidence.dot(normal) * 2).normalize();
}

Vector RayTracer::refract(const Vector& incidence, const Vector& normal, 
    double index_of_refraction, bool air)
{
    index_of_refraction = 0.85; // use default, TODO: delete later!
    double a = air ? (1.0 / index_of_refraction) : index_of_refraction;
    double cos1 = normal.dot(incidence) * normal.dot(incidence);
    double cos2 = sqrt(a * cos1 - a + 1.0);
    return (incidence * a + normal * (normal.dot(incidence) * a - cos2))
        .normalize();
}

Vector RayTracer::diffuse(const Vector& normal)
{
    Vector res;
    do {
        res.x = drand48() * 2.0 - 1.0;
        res.y = drand48() * 2.0 - 1.0;
        res.z = drand48() * 2.0 - 1.0;
        res.normalize();
    } while (res.dot(normal) >= 0);

    return res;
}
