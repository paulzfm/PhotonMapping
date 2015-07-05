#ifndef RAY_TRACER_H_
#define RAY_TRACER_H_

#include "Scene.h"
#include "Camera.h"
#include "util/Image.h"
#include "util/PhotonMap.h"
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

    // build photon map: call before rendering
    void buildPhotonMap();
    
    // render
    void render(int n_sample);
    void fastRender();
    
    void renderMap(); // debug
    void renderMap2(); // debug

private:
    std::unique_ptr<Scene> _scene;
    std::unique_ptr<Image> _img;
    std::unique_ptr<Material> _env;

    // cameras
    std::vector< std::unique_ptr<Camera> > _cameras;

    // photon map
    std::unique_ptr<PhotonMap> _photons_map;

    // image size
    int _width, _height;

    // parameters
    int _num_photons;
    int _max_photon_bounce;
    int _max_tracing_depth;
    double _gathering_radius;
    double _exposure;

    // parse parameters
    void parseParams(const JsonBox::Value& val);

    // compute pixel's color
    RGB pixelColor(const Ray& ray, int depth = 0, double relevance = 1.0);

    // photon bouncing
    void bounce(Photon& photon, int bounces);

    // helper functions
    double getReflectance(const Vector &dir, const Vector &normal, double from, double to);
    Vector reflect(const Vector& incidence, const Vector& normal);
    Vector refract(const Vector& incidence, const Vector& normal, 
        double from, double to);
    Vector diffuse(const Vector& normal, double roughness);
};

#endif // RAY_TRACER_H_