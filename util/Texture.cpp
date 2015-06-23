#include "Texture.h"

RGB Texture::value(const Point& uv)
{
    double u = uv.x - (int)uv.x;
    double v = uv.y - (int)uv.y; 
    u *= _img.width() - 3;
    v *= _img.height() - 3;

    int iu = (int)u;
    int iv = (int)v;

    double tu = u - iu;
    double tv = v - iv;

    RGB color = 
        _img.get(iu, iv) * (1 - tu) * (1 - tv) +
        _img.get(iu + 1, iv) * tu * (1 - tv) +
        _img.get(iu, iv + 1) * (1 - tu) * tv +
        _img.get(iu + 1, iv + 1) * tu * tv;

   return color;
}