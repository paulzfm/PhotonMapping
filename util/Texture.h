#ifndef UTIL_TEXTURE_H_
#define UTIL_TEXTURE_H_

#include "Image.h"
#include "Point.h"

class Texture
{
public:
    Texture(const std::string& file) : _img(file) {}

    RGB value(const Point& uv);

    int width() const { return _img.width(); }
    int height() const { return _img.height(); }

private:
    Image _img;
};


#endif