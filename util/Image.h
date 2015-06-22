#ifndef UTIL_IMAGE_H_
#define UTIL_IMAGE_H_

#include "RGB.h"
#include <string>

class Image
{
public:
    Image(int width, int height);

    // Get size.
    int width() const  { return _width;  };
    int height() const { return _height; };

    // Set pixel with the given color, return false if out of range.
    void set(int index, const RGB& color);

    void gammaCorrect(double gamma);

    // File I/O as .ppm format.
    void loadPPM(std::string path);
    void dumpPPM(std::string path);

private:
    RGB* _raster;
    int _width;
    int _height;
};

#endif // UTIL_IMAGE_H_