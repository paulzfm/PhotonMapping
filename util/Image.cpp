#include "Image.h"
#include "common.h"

#include <stdlib.h>
#include <math.h>
#include <fstream>
#include <iostream>

Image::Image(int width, int height)
    : _width(width), _height(height)
{
    _raster = std::unique_ptr<RGB[]>(new RGB[width * height]);
}

Image::Image(const std::string& file)
{
    loadPPM(file);
}

void Image::set(int index, const RGB& color)
{
    _raster[index] = color;
}

void Image::gammaCorrect(double gamma)
{
    // RGB tmp;
    // double power = 1.0 / gamma;
    // for (int i = 0; i < _width; i++) {
    //     for (int j = 0; j < _height; j++) {
    //         tmp = _raster[i][j];
    //         _raster[i][j] = RGB(
    //             pow(tmp.r, power),
    //             pow(tmp.g, power),
    //             pow(tmp.b, power)
    //         );
    //     }
    // }
}

void Image::loadPPM(const std::string& path)
{
    // open file
    std::ifstream in(path);
    if (!in) {
        std::cerr << "Error: failed to open file \"" << path << "\".\n";
        exit(1);
    }

    char tmp;
    int num;
    unsigned char red, green, blue;

    // read header
    in >> tmp >> tmp;
    in >> _width >> _height >> num;

    // allocate raster
    _raster = std::unique_ptr<RGB[]>(new RGB[_width * _height]);

    // new line
    in >> tmp;

    // read pixels
    for (int i = 0; i < _width * _height; i++) {
        in >> red >> blue >> green;
        _raster[i] = RGB(
            (double)(red / 255.0),
            (double)(green / 255.0),
            (double)(blue / 255.0)
        );
    }

    // close file
    in.close();
}

void Image::dumpPPM(const std::string& path)
{
    // open file
    std::ofstream out(path);
    if (!out) {
        std::cerr << "Error: failed to open file \"" << path << "\".\n";
        exit(1);
    }

    // write header
    out << "P6\n";
    out << _width << " " << _height << "\n";
    out << "255\n";
    
    // write pixels
    for (int i = 0; i < _width * _height; i++) {
        unsigned int red = (unsigned int)(_raster[i].r * 255);
        unsigned int green = (unsigned int)(_raster[i].g * 255);
        unsigned int blue = (unsigned int)(_raster[i].b * 255);
        out.put((unsigned char)red);
        out.put((unsigned char)green);
        out.put((unsigned char)blue);
    }

    // close file
    out.close();
}

