#ifndef IMAGE_H_
#define IMAGE_H_

#include "RGB.h"
#include <string>

/*class Image
{
public:
    Image() {}
    Image(int width, int height);
    Image(int width, int height, const RGB& background);
    ~Image();

    // Get size.
    int width() const  { return _width;  };
    int height() const { return _height; };

    // Get color.
    RGB colorAt(int x, int y) const { return _raster[x][y]; };

    // Set pixel with the given color, return false if out of range.
    bool set(int x, int y, const RGB& color);

    void gammaCorrect(double gamma);

    // File I/O as .ppm format.
    void loadPPM(std::string path);
    void dumpPPM(std::string path);

private:
    RGB** _raster;
    int _width;
    int _height;
};*/

class Image {
public:
    Image(int width, int height);
    ~Image();

    // if scale_color is true, the output targa will have its color space scaled
    // to the global max, otherwise it will be clamped at 1.0
    void WriteTga(char *outfile, bool scale_color = true);

    void GenTestPattern();

    // property accessors
    RGB pixel(int x, int y);
    void set(int x, int y, RGB pxl);
    int width() const { return _width; }
    int height() const { return _height; }
    double max() const { return _max; }

private:
    int _width;
    int _height;
    RGB **_pixmap;
    double _max;
};

#endif // IMAGE_H_