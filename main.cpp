#include "Scene.h"
#include "RayTracer.h"
#include "util/Image.h"
#include "util/shape/Sphere.h"
#include "util/shape/Plane.h"

#include "util/Vector.h"

int main(int argc, char** argv)
{
    // debug();exit(1);

    RayTracer tracer;
    tracer.setup("scenes/balls.json");
    printf("building...\n");
    tracer.buildGlobalMap();
    printf("rendering...\n");
    tracer.renderMap();
    printf("\nDone.\n");
     
    return 0;
}