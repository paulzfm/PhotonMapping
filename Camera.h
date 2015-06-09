// A straightforward camera with square lens.

#ifndef CAMERA_H_
#define CAMERA_H_

#include "util/Vector.h"
#include "util/ONB.h"
#include "util/Ray.h"

class Camera
{
public:
    Camera() {}
    Camera(const Camera& c);
    Camera(Vector& center, Vector& gaze, Vector& up, 
        double aperture, double distance,
        double left, double right, double bottom, double top);

    // Ray at pixel (a, b). x1 and x2 are lens samples in the range (0, 1) * (0, 1)
    Ray rayAt(double a, double b, double x1, double x2) const;

    Vector center, corner, across, up;
    ONB uvw;
    double r; // radius
    double u0, u1, v0, v1;
    double d;
};

#endif // CAMERA_H_