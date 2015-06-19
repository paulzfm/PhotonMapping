#ifndef RAY_TRACER_H_
#define RAY_TRACER_H_

#include "Scene.h"
#include "Camera.h"
#include "util/Image.h"
#include "util/Map.h"
#include "util/light/PointLight.h"

#include <vector>

#define GLOBAL_MAP 0
#define CAUSTICS_MAP 1
#define BOTH_MAPS 2

#define NUM_GLOBAL_PHOTONS 10000
#define NUM_CAUSTICS_PHOTONS 10000
#define MAX_DEPTH 3
#define Ia 1.0 //ambient
#define Ka 0.2 //ambient
#define Id 1.0 //diffuse
#define Kd 0.5 //diffuse
#define Is 1.0 //specular
#define Ks 0.4 //specular
#define specular_power 7.0 //specular
#define Kp 0.2 //Contribution from PhotonMapping
#define Kc 0.2

class RayTracer
{
public:
    RayTracer(Scene& scene)
        : _scene(scene), 
          _global_map(NUM_GLOBAL_PHOTONS * 10),
          _caustics_map(NUM_CAUSTICS_PHOTONS * 10) {}

    // call this first to build global map and caustics map
    void buildGlobalMap(int num_of_bounces);
    void buildCausticsMap(int num_of_bounces);
    
    // call this later(after building and setting) to render
    void render();
    void distributionRender();
    void renderMap();

    // setters
    void setCamera(Camera& camera);
    void setImage(Image* img);

    PointLight light;
    Vector eye;

// private:
public:
    Scene _scene;
    Camera* _camera;
    Image* _img;

    int _width, _height;

    // maps
    Map _global_map, _caustics_map;

    // compute pixel's color
    RGB pixelColor(const Ray& ray, int depth);

    // store photon
    void storePhoton(int type, const RGB& power, const Vector& pos, const Vector& dir);

    // look up photon map
    RGB lookUpMap(int type, const Vector& center, double radius, 
        const Vector& normal);

    // helper functions
    Vector reflect(const Vector& incidence, const Vector& normal);
    Vector refract(const Vector& incidence, const Vector& normal, 
        double index_of_refraction, bool air);
    Vector diffuse(const Vector& normal, double roughness);
};

#endif // RAY_TRACER_H_