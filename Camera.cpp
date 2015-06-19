#include "Camera.h"

#include <math.h>

const std::string Camera::CLS = "camera";

Camera::Camera(const Vector& origin, const Vector& direction, const Vector& top, 
    double fovy, int width, int height)
    : _origin(origin), _dir(direction), _width(width), _height(height)
{
    _aspect = (double)width / height;
    _half_height = height / 2.0;
    _half_width = width / 2.0;

    _top = top * tan(fovy);
    _right = _dir.cross(top) * tan(fovy) * (-_aspect);
}

Ray Camera::rayAt(double x, double y) const
{
    double dx = (x + 0.5 - _half_width) / _half_width;
    double dy = (-y + 0.5 + _half_height) / _half_height;

    return Ray(_origin, (_dir + _top * dy + _right * dx).normalize());
}

std::unique_ptr<Camera> Camera::parse(const JsonBox::Value& val)
{
    Parser::checkObject(val, CLS);
    
    JsonBox::Object obj = val.getObject();
    Parser::checkParam(obj, CLS, "origin", Parser::VEC3);
    Parser::checkParam(obj, CLS, "direction", Parser::VEC3);
    Parser::checkParam(obj, CLS, "top", Parser::VEC3);
    Parser::checkParam(obj, CLS, "fovy", Parser::NUMBER);
    Parser::checkParam(obj, CLS, "width", Parser::INTEGER);
    Parser::checkParam(obj, CLS, "height", Parser::INTEGER);

    return std::unique_ptr<Camera>(new Camera(
        Parser::asVector(obj["origin"]),
        Parser::asVector(obj["direction"]),
        Parser::asVector(obj["top"]),
        Parser::asNumber(obj["fovy"]) * 2 * PI / 360.0,
        Parser::asInteger(obj["width"]),
        Parser::asInteger(obj["height"])
    ));
}