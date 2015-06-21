#ifndef RAY_TRACER_H_
#define RAY_TRACER_H_

#include "Scene.h"
#include "Camera.h"
#include "util/Image.h"
#include "util/Map.h"
#include "util/Material.h"

#include <vector>

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

    // set output: call before runnning
    void setImgSize(int width, int height);

    // build global map and caustics map: call before rendering
    void buildGlobalMap();
    void buildCausticsMap();
    
    // render
    void render();
    void distributionRender();
    void renderMap();

    // change a new camera
    void changeCamera(std::unique_ptr<Camera> camera);

private:
    std::unique_ptr<Scene> _scene;
    std::unique_ptr<Camera> _camera;
    std::unique_ptr<Image> _img;
    std::unique_ptr<Material> _env;

    // maps
    std::unique_ptr<Map> _global_map, _caustics_map;

    // image size
    int _width, _height;

    // parameters
    int _num_global_photons;
    int _max_photon_bounce;
    int _max_tracing_depth;
    double _gathering_radius;
    double _exposure;

    // compute pixel's color
    RGB pixelColor(const Ray& ray, int depth);

    // photon bouncing
    void globalBounce(const Ray& ray, RGB& power);

    // store photon
    void storePhoton(int type, const RGB& power, const Vector& pos, const Vector& dir);

    // look up photon map
    RGB lookUpMap(int type, const Vector& center, double radius, 
        const Vector& normal);

    // helper functions
    double reflectance(const Vector &dir, const Vector &normal, double from, double to);
    Vector reflect(const Vector& incidence, const Vector& normal);
    Vector refract(const Vector& incidence, const Vector& normal, 
        double from, double to);
    Vector diffuse(const Vector& normal, double roughness);
};

#endif // RAY_TRACER_H_