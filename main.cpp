#include "Scene.h"
#include "RayTracer.h"
#include "util/Image.h"
#include "util/shape/Sphere.h"
#include "util/shape/Plane.h"
#include "util/shape/Triangle.h"

#include "util/Vector.h"
#include "util/Sample.h"
#include "util/Image.h"

int test()
{
    Triangle t(Vector(0,0,0), Vector(1,0,0), Vector(1,0,1));
    Ray r(Vector(0.5, 0.5, 0.5), Vector(0.1,1,0.1));
    HitRecord rec;
    if (t.hit(r, 0, rec)) {
        std::cout << rec.n << " " << r.o + rec.t * r.d << std::endl;
    } else {
        printf("MISS\n");
    }

    return 0;
}

int main(int argc, char** argv)
{
    // return test();

    RayTracer tracer;
    tracer.setup("scenes/model.json");
    // tracer.setup("scenes/box.json");
    tracer.buildPhotonMap();
    tracer.fastRender();
    // tracer.render();
    // tracer.renderMap();
    printf("\nDone.\n");
     
    return 0;
}

