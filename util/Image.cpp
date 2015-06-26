#include "Image.h"
#include "common.h"

#include <stdlib.h>
#include <math.h>
#include <fstream>
#include <iostream>
#include <sstream>

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
    if (index >= _height * _width || index < 0) {
        std::cerr << "Error: pixel index out of range " << index << ".\n";
        exit(1);
    }

    _raster[index] = color;
}

RGB Image::get(int x, int y)
{
    if (x >= _width || y >= _height) {
        std::cerr << "Error: pixel position out of range (" << x << ", " << y << ").\n";
        exit(1);
    }

    return _raster[x * _height + y];
}

// refer to:
//     http://blog.csdn.net/chenwk891/article/details/9330991
void Image::loadPPM(const std::string& path)
{
    // open file
    std::ifstream in(path);
    if (!in) {
        std::cerr << "Error: failed to open file \"" << path << "\".\n";
        exit(1);
    }

    std::string tmp;
    if (getline(in, tmp, '\n')) {
        if (tmp != "P6") {
            std::cerr << "Error: invalid ppm header in\"" << path << "\".\n";
            exit(1);
        } 
    } else {
        std::cerr << "Error: cannot read the ppm type in\"" << path << "\".\n";
        exit(1);
    }
  
    // skip comments
    while (getline(in, tmp))
    {  
        if (tmp[0] == '#') {
            continue;  
        } else {
            break;
        }
    }
  
    // read image size  
    std::istringstream iss;
    iss.str(tmp);
    iss >> _width >> _height;
    if (_width <= 0 || _height <= 0) {
        std::cerr << "Error: invalid image size in\"" << path << "\".\n";
        exit(1);
    }

    // allocate raster
    _raster = std::unique_ptr<RGB[]>(new RGB[_width * _height]);
  
    // maximum value (usually 255)  
    unsigned int max_value;
    if (getline(in, tmp)) {
        iss.clear();
        iss.str(tmp);
        iss >> max_value;
        if (max_value <= 0) {
            std::cerr << "Error: invalid maximum value in\"" << path << "\".\n";
        }
    } else {
        std::cerr << "Error: cannot read maximum value in\"" << path << "\".\n";
    }

    double inv_max_value = 1.0 / (double)max_value;
  
    // read pixel data
    std::string rgb((std::istreambuf_iterator<char>(in)), std::istreambuf_iterator<char>());
    if (rgb.size() / 3 != _width * _height) {
        std::cerr << "Error: invalid pixel number in\"" << path << "\".\n";
    }
    
    // read rgb values from string
    unsigned char red, green, blue;
    double r, g, b;
    for (unsigned int i = 0; i < _width * _height; ++i) {
        red     = rgb[3 * i];
        green   = rgb[3 * i + 1];
        blue    = rgb[3 * i + 2];
        r = red   * inv_max_value;
        g = green * inv_max_value;
        b = blue  * inv_max_value;
        _raster[i] = RGB(r, g, b);
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

