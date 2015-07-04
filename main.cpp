#include "Scene.h"
#include "RayTracer.h"
#include "util/Image.h"
#include "util/shape/Sphere.h"
#include "util/shape/Plane.h"
#include "util/shape/Triangle.h"
#include "util/shape/BBox.h"
#include "util/shape/Box.h"

#include "util/Vector.h"
#include "util/Sample.h"
#include "util/Image.h"

#include <stdio.h>
#include <string.h>
#include <string>

// int test()
// {
//     BBox bbox(Vector(0,0,0), Vector(2,2,2));
//     Box box(Vector(0,0,0), Vector(0,2,0), Vector(0,0,2), Vector(2,0,0));
//     Ray ray(Vector(1,3,1), Vector(0,-1,0));
//     if (bbox.shadowHit(ray)) {
//         printf("HIT\n");
//     } else {
//         printf("MISS\n");
//     }

//     HitRecord rec;
//     if (box.hit(ray, 0, rec)) {
//         printf("HIT\n");
//     } else {
//         printf("MISS\n");
//     }

//     return 0;
// }

void run(const std::string& file, int samples)
{
    RayTracer tracer;
    tracer.setup(file);
    tracer.buildPhotonMap();

    if (samples == 0) {
        tracer.fastRender();
    } else {
        tracer.render();
    }

    printf("--> All done\n");
}

int main(int argc, char** argv)
{
    if (argc != 2 && argc != 4) {
        fprintf(stderr, "Usage: %s <scene file> [-s samples]\n", argv[0]);
        exit(1);
    }
    
    std::string file(argv[1]);

    int samples = 0;
    if (argc == 4) {
        if (strcmp(argv[2], "-s") != 0) {
            fprintf(stderr, "Arg error: invalid option \"%s\", \"-s\" expected.\n", argv[2]);
            exit(1);
        } else {
            samples = atoi(argv[3]);
            if (samples <= 0) {
                fprintf(stderr, "Arg warning: fast render will be applied for option: \"-s %d\"\n", samples);
            }
        }
    }

    run(file, samples);

    return 0;
}

