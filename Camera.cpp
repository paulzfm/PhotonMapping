#include "Camera.h"

Camera::Camera(Vector& center, Vector& gaze, Vector& up, 
    double aperture, double distance,
    double left, double right, double bottom, double top)
    : center(center), d(distance), 
      u0(left), u1(right), v0(bottom), v1(top)
{
    r = aperture / 2.0;
    uvw.fromWV(-gaze, up);
    corner = center + u0 * uvw.u + v0 * uvw.v - d * uvw.w;
    across = (u0 - u1) * uvw.u;
    up = (v0 - v1) * uvw.v;
}

Ray Camera::rayAt(double a, double b, double x1, double x2) const
{
    Vector origin = center + 2.0 * (x1 - 0.5) * r * uvw.u + 
        2.0 * (x2 - 0.5) * r * uvw.v;
    Vector target = corner + across * a + up * b;
    return Ray(origin, (target - origin).normalize());
}