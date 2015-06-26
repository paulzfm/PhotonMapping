#include "Scene.h"
#include "RayTracer.h"
#include "util/Image.h"
#include "util/shape/Sphere.h"
#include "util/shape/Plane.h"

#include "util/Vector.h"
#include "util/Sample.h"
#include "util/Image.h"

int test()
{
    // Image img("wood.ppm");
    // for (int i = 0; i < img.width(); i++) {
    //     for (int j = 0; j < img.height(); j++) {
    //         img.get(i, j);
    //     }
    // }

    // int n = 5;
    // std::vector<Point> samples;
    // double inv = 1.0 / n;
    //     double offset = inv / 2.0;
    //     for (int i = 0; i < n; ++i) {
    //         for (int j = 0; j < n; ++j) {
    //             samples.push_back(Point(offset + inv * i - 0.5, offset + inv * j - 0.5));
    //         }
    //     }

    // for (const auto& s : samples) {
    //     std::cout << s << std::endl;
    // }

    return 0;
}

int main(int argc, char** argv)
{
    // return test();

    RayTracer tracer;
    tracer.setup("scenes/glass.json");
    tracer.buildPhotonMap();
    tracer.fastRender();
    // tracer.render();
    // tracer.renderMap();
    printf("\nDone.\n");
     
    return 0;
}

