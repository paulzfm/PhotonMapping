#ifndef UTIL_IMAGE_H_
#define UTIL_IMAGE_H_

#include "RGB.h"

#include <string>
#include <memory>

class Image
{
public:
    Image(const std::string& file);
    Image(int width, int height);

    // Get size.
    int width() const  { return _width;  };
    int height() const { return _height; };

    // Set pixel with the given color.
    void set(int index, const RGB& color);

    // Get color at pixel (x, y).
    RGB get(int x, int y);

    // File I/O as .ppm format.
    void loadPPM(const std::string& path);
    void dumpPPM(const std::string& path);

private:
    std::unique_ptr<RGB[]> _raster;
    int _width;
    int _height;
};

#endif // UTIL_IMAGE_H_