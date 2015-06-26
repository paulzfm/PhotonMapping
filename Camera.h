#ifndef CAMERA_H_
#define CAMERA_H_

#include "util/Vector.h"
#include "util/Ray.h"
#include "util/Parser.h"

#include <string>
#include <memory>

class Camera
{
public:
    Camera(const Vector& origin, const Vector& direction, const Vector& top, 
        double fovy, int width, int height, const std::string& output);

    // Ray at pixel (x, y)
    Ray rayAt(double x, double y) const;

    // parse json object
    static std::unique_ptr<Camera> parse(const JsonBox::Value& val);

    const static std::string CLS;

    int width, height; // size
    std::string output; // output file path

private:
    double _aspect; // width / height
    double _half_width, _half_height;

    Vector _origin, _dir, _top, _right;
};

#endif // CAMERA_H_