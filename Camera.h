#ifndef CAMERA_H_
#define CAMERA_H_

#include "util/Vector.h"
#include "util/Ray.h"
#include "util/Parser.h"

class Camera
{
public:
    Camera(const Vector& origin, const Vector& direction, const Vector& top, 
        double fovy, int width, int height);

    // Ray at pixel (x, y)
    Ray rayAt(double x, double y) const;

    // parse json object
    static Camera parse(const JsonBox::Value& val);

    const static std::string CLS;

private:
    int _width, _height; // size
    double _aspect; // width / height
    double _half_width, _half_height;

    Vector _origin, _dir, _top, _right;
};

#endif // CAMERA_H_