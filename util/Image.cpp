#include "Image.h"
#include "common.h"

#include <stdlib.h>
#include <math.h>
#include <fstream>
#include <iostream>

/*
Image::Image(int width, int height)
    : _width(width), _height(height)
{
    _raster = new RGB*[width];
    for (int i = 0; i < width; i++) {
        _raster[i] =  new RGB[height];
    }
}

Image::Image(int width, int height, const RGB& background)
    : _width(width), _height(height)
{
    _raster = new RGB*[width];
    for (int i = 0; i < width; i++) {
        _raster[i] =  new RGB[height];
    }

    for (int i = 0; i < width; i++) {
        for (int j = 0; j < height; j++) {
            _raster[i][j] = background;
        }
    }
}

Image::~Image()
{
    if (_raster) {
        for (int i = 0; i < _width; i++) {
            delete[] _raster[i];
        }
        delete[] _raster;
    }
}

bool Image::set(int x, int y, const RGB& color)
{
    if (0 <= x && x < _width && 0 <= y && y < _height) {
        _raster[x][y] = color;
        return true;
    }

    return false; // out of bound
}

void Image::gammaCorrect(double gamma)
{
    RGB tmp;
    double power = 1.0 / gamma;
    for (int i = 0; i < _width; i++) {
        for (int j = 0; j < _height; j++) {
            tmp = _raster[i][j];
            _raster[i][j] = RGB(
                pow(tmp.r, power),
                pow(tmp.g, power),
                pow(tmp.b, power)
            );
        }
    }
}

void Image::loadPPM(std::string path)
{
    // open file
    std::ifstream in(path);
    if (!in) {
        std::cerr << "Error: failed to open file [" << path << "].\n";
        exit(1);
    }

    char tmp;
    int num;
    unsigned char red, green, blue;

    // read header
    in >> tmp >> tmp;
    in >> _width >> _height >> num;

    // allocate raster
    _raster = new RGB*[_width];
    for (int i = 0; i < _width; i++) {
        _raster[i] =  new RGB[_height];
    }

    // new line
    in >> tmp;

    // read pixels
    for (int i = 0; i < _width; i++) {
        for (int j = _height - 1; j >= 0; j--) {
            in >> red >> blue >> green;
            _raster[i][j] = RGB(
                (double)(red / 255.0),
                (double)(green / 255.0),
                (double)(blue / 255.0)
            );
        }
    }

    // close file
    in.close();
}

void Image::dumpPPM(std::string path)
{
    // open file
    std::ofstream out(path);
    if (!out) {
        std::cerr << "Error: failed to open file [" << path << "].\n";
        exit(1);
    }

    // write header
    out << "P6\n";
    out << _width << " " << _height << "\n";
    out << "255\n";
    
    // write pixels
    // double exposure = -1.0 / 255.0;
    for (int i = 0; i < _width; i++) {
        for (int j = _height - 1; j >= 0; j--) {
            // unsigned int red = (unsigned int)((1.0 - exp(_raster[i][j].r * exposure)) * 255);
            // unsigned int green = (unsigned int)((1.0 - exp(_raster[i][j].g * exposure)) * 255);
            // unsigned int blue = (unsigned int)((1.0 - exp(_raster[i][j].b * exposure)) * 255);
            unsigned int red = (unsigned int)(_raster[i][j].r * 255);
            unsigned int green = (unsigned int)(_raster[i][j].g * 255);
            unsigned int blue = (unsigned int)(_raster[i][j].b * 255);
            red = MIN(red, 255);
            green = MIN(green, 255);
            blue = MIN(blue, 255);
            out.put((unsigned char)red);
            out.put((unsigned char)green);
            out.put((unsigned char)blue);
        }
    }

    // close file
    out.close();
}
*/

Image::Image(int width, int height)
{
    _width = width;
    _height = height;
    _max = 1.0;

    // allocate the first dimension, "width" number of RGB pointers...
    _pixmap = (RGB **)malloc(sizeof(RGB *) * _width);

    // allocate the second dimension, "height" number of RGB structs...
    for (int i = 0; i < _width; i++)
    {
        _pixmap[i] = (RGB *)malloc(sizeof(RGB) * _height);
    }
}

Image::~Image()
{
    // free each column of pixels first...
    for (int i = 0; i < _width; i++)
    {
        free(_pixmap[i]);
    }

    // free the rows of pixels second...
    free(_pixmap);
}

void Image::WriteTga(char *outfile, bool scale_color)
{
    FILE *fp = fopen(outfile, "w");
    if (fp == NULL)
    {
        perror("ERROR: Image::WriteTga() failed to open file for writing!\n");
        exit(EXIT_FAILURE);
    }
    
    // write 24-bit uncompressed targa header
    // thanks to Paul Bourke (http://local.wasp.uwa.edu.au/~pbourke/dataformats/tga/)
    putc(0, fp);
    putc(0, fp);
    
    putc(2, fp); // type is uncompressed RGB
    
    putc(0, fp);
    putc(0, fp);
    putc(0, fp);
    putc(0, fp);
    putc(0, fp);
    
    putc(0, fp); // x origin, low byte
    putc(0, fp); // x origin, high byte
    
    putc(0, fp); // y origin, low byte
    putc(0, fp); // y origin, high byte

    putc(_width & 0xff, fp); // width, low byte
    putc((_width & 0xff00) >> 8, fp); // width, high byte

    putc(_height & 0xff, fp); // height, low byte
    putc((_height & 0xff00) >> 8, fp); // height, high byte

    putc(24, fp); // 24-bit color depth

    putc(0, fp);

    // write the raw pixel data in groups of 3 bytes (BGR order)
    for (int y = 0; y < _height; y++)
    {
        for (int x = 0; x < _width; x++)
        {
            // if color scaling is on, scale 0.0 -> _max as a 0 -> 255 unsigned byte
            unsigned char rbyte, gbyte, bbyte;
            if (scale_color)
            {
                rbyte = (unsigned char)((_pixmap[x][y].r / _max) * 255);
                gbyte = (unsigned char)((_pixmap[x][y].g / _max) * 255);
                bbyte = (unsigned char)((_pixmap[x][y].b / _max) * 255);
            }
            else
            {
                double r = (_pixmap[x][y].r > 1.0) ? 1.0 : _pixmap[x][y].r;
                double g = (_pixmap[x][y].g > 1.0) ? 1.0 : _pixmap[x][y].g;
                double b = (_pixmap[x][y].b > 1.0) ? 1.0 : _pixmap[x][y].b;
                rbyte = (unsigned char)(r * 255);
                gbyte = (unsigned char)(g * 255);
                bbyte = (unsigned char)(b * 255);
            }
            putc(bbyte, fp);
            putc(gbyte, fp);
            putc(rbyte, fp);
        }
    }

    fclose(fp);
}

void Image::GenTestPattern()
{
    RGB pxl(0.0, 0.0, 0.0, 0.0);
    int i, j, color;
    float radius, dist;
    
    // draw a rotating color checkerboard (RGB) in a 25x25 pixel grid
    for (int x = 0; x < _width; x++)
    {
        for (int y = 0; y < _height; y++)
        {
            i = x / 25;
            j = y / 25;
            color = (i + j) % 3;
            
            switch (color)
            {
                case 0: // red
                    pxl.r = 1.0; pxl.g = 0.0; pxl.b = 0.0;
                    break;

                case 1: // green
                    pxl.r = 0.0; pxl.g = 1.0; pxl.b = 0.0;
                    break;

                case 2: // blue
                    pxl.r = 0.0; pxl.g = 0.0; pxl.b = 1.0;
                    break;
            }

            set(x, y, pxl);
        } 
    }

    // draw a black circle in the top left quadrant (centered at (i, j))
    pxl.r = 0.0; pxl.g = 0.0; pxl.b = 0.0;
    i = _width / 4;
    j = 3 * _height / 4;
    radius = (((float)_width / 4.0) < ((float)_height / 4.0)) ? (float)_width / 4.0 : (float)_height / 4.0;
    for (int x = 0; x < _width; x++)
    {
        for (int y = 0; y < _height; y++)
        {
            dist = sqrtf((float)((x - i) * (x - i)) + (float)((y - j) * (y - j)));
            if (dist <= (float)radius)
            {
                set(x, y, pxl);
            }
        }
    }
    
    // draw a white circle in the lower right quadrant (centered at (i, j))
    pxl.r = 1.0; pxl.g = 1.0; pxl.b = 1.0;
    i = 3 * _width / 4;
    j = _height / 4;
    radius = (((float)_width / 4.0) < ((float)_height / 4.0)) ? (float)_width / 4.0 : (float)_height / 4.0;
    for (int x = 0; x < _width; x++)
    {
        for (int y = 0; y < _height; y++)
        {
            dist = sqrtf((float)((x - i) * (x - i)) + (float)((y - j) * (y - j)));
            if (dist <= (float)radius)
            {
                set(x, y, pxl);
            }
        }
    }
}

RGB Image::pixel(int x, int y)
{
    if (x < 0 || x > _width - 1 ||
        y < 0 || y > _height - 1)
    {
        // catostrophically fail
        fprintf(stderr, "ERROR: Image::pixel(%d, %d) outside range of the image!\n", x, y);
        exit(EXIT_FAILURE);
    }
    
    return _pixmap[x][y];
}

void Image::set(int x, int y, RGB pxl)
{
    if (x < 0 || x > _width - 1 ||
        y < 0 || y > _height - 1)
    {
        // catostrophically fail
        fprintf(stderr, "ERROR: Image::pixel(%d, %d, pixel) outside range of the image!\n", x, y);
        exit(EXIT_FAILURE);
    }
    
    _pixmap[x][y] = pxl;
    //if (x==320)
    //   std::cout << '(' << x << ',' << y << ')' << '\n';
    // update the max color if necessary
    _max = (pxl.r > _max) ? pxl.r : _max;
    _max = (pxl.g > _max) ? pxl.g : _max;
    _max = (pxl.b > _max) ? pxl.b : _max;
}