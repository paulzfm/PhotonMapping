#include "Scene.h"
#include "RayTracer.h"
#include "util/Image.h"
#include "util/shape/Sphere.h"
#include "util/shape/Plane.h"

#include "util/Vector.h"
#include "util/Sample.h"


int test()
{
    std::vector<Point> points(9);
    Sample::jitter(points, 9);
    for (const auto p : points) {
        std::cout << p << std::endl;
    }

    return 0;
}

int main(int argc, char** argv)
{
    // return test();

    RayTracer tracer;
    tracer.setup("scenes/glass.json");
    printf("building...\n");
    tracer.buildGlobalMap();
    tracer.buildCausticsMap();
    printf("rendering...\n");
    tracer.fastRender();
    // tracer.render();
    printf("\nDone.\n");
     
    return 0;
}