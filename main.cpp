#include "Scene.h"
#include "RayTracer.h"

#include <stdio.h>
#include <string.h>
#include <string>

void run(const std::string& file, int samples)
{
    RayTracer tracer;
    tracer.setup(file);
    tracer.buildPhotonMap();

    if (samples == 0) {
        tracer.fastRender();
    } else {
        tracer.render(samples);
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

