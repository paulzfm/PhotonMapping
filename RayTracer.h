#ifndef RAY_TRACER_H_
#define RAY_TRACER_H_

#include "Scene.h"
#include "Camera.h"
#include "util/Image.h"
#include "util/KDT.h"
#include "util/Material.h"

#include <vector>
#include <memory>

#define GLOBAL_MAP 0
#define CAUSTICS_MAP 1
#define BOTH_MAPS 2

class RayTracer
{
public:
    RayTracer()
        : _scene(std::unique_ptr<Scene>(new Scene)) {}

    // setup scene & camera: call before runnning
    void setup(const std::string& file);

    // build global map and caustics map: call before rendering
    void buildGlobalMap();
    void buildCausticsMap();
    
    // render
    void render();
    void fastRender();
    void renderMap();
    void renderMap2();

// private:
    std::unique_ptr<Scene> _scene;
    std::unique_ptr<Image> _img;
    std::unique_ptr<Material> _env;

    // cameras
    std::vector< std::unique_ptr<Camera> > _cameras;

    // maps
    std::unique_ptr<KDT> _global_map, _caustics_map;

    // image size
    int _width, _height;

    // parameters
    int _num_global_photons;
    int _num_caustics_photons;
    int _max_photon_bounce;
    int _max_tracing_depth;
    double _gathering_radius;
    double _exposure;

    int _samples_per_pixel;

    // parse parameters
    void parseParams(const JsonBox::Value& val);

    // compute pixel's color
    RGB pixelColor(const Ray& ray, int depth = 0, double relevance = 1.0);

    // photon bouncing
    void globalBounce(Photon& photon, int bounces);
    void causticsBounce(Photon& photon, int bounces);

    // store photon
    void storePhoton(int type, const Photon& photon);

    // look up photon map
    RGB lookUpMap(int type, const Vector& center, double radius, 
        const Vector& normal);

    // helper functions
    double getReflectance(const Vector &dir, const Vector &normal, double from, double to);
    Vector reflect(const Vector& incidence, const Vector& normal);
    Vector refract(const Vector& incidence, const Vector& normal, 
        double from, double to);
    Vector diffuse(const Vector& normal, double roughness);
};

#endif // RAY_TRACER_H_