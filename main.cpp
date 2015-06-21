#include "Scene.h"
#include "RayTracer.h"
#include "util/Image.h"
#include "util/shape/Sphere.h"
#include "util/shape/Plane.h"

/*
void testCornell()
{
    // walls
    Plane ceiling(Vector(0, -1, 0), 600);
    ceiling.color = RGB(1, 1, 1);
    Plane ground(Vector(0, 1, 0), 300);
    ground.color = RGB(1, 1, 1);
    Plane back(Vector(0, 0, 1), 700);
    back.color = RGB(1, 1, 1);
    Plane left(Vector(1, 0, 0), 400);
    left.color = RGB(1, 0, 0);
    Plane right(Vector(-1, 0, 0), 400);
    right.color = RGB(0, 1, 0);

    // sphere
    Sphere sphere(Vector(200, -200, -400), 100);
    sphere.color = RGB(1, 1, 1);

    // add objects
    Scene myScene;
    myScene.addShape(&ceiling);
    myScene.addShape(&ground);
    myScene.addShape(&back);
    myScene.addShape(&left);
    myScene.addShape(&right);
    myScene.addShape(&sphere);

    // make a 640x480 image
    Image img(640, 480);

    // tracer
    RayTracer tracer(myScene);
    tracer.eye = Vector(0.0f,0.0f,0.0f);
    tracer.light.pos = Vector(300.0f, 400.0f, -100.0f);

    tracer.setImage(&img);
    printf("building...\n");
    tracer.buildGlobalMap(3);
    // tracer.buildCausticsMap(3);
    printf("rendering...\n");
    tracer.render();
    // tracer.renderMap();
    printf("done\n");

    img.WriteTga("cornell.tga", true);
}

void test()
{
    // spheres
    Vector center1(0.0f, -150.0f, -400.0f);
    RGB sphereColor1(1.0f,1.0f,1.0f,0.1f);
    Sphere sphere1(center1, 100.0f);
    sphere1.color = sphereColor1;
    sphere1.reflectance = 0.0;

    Vector center2(-200.0f, 100.0f, -500.0f);
    RGB sphereColor2(1.0f,0.0f,0.0f);
    Sphere sphere2(center2, 100.0f);
    sphere2.color = sphereColor2;
    sphere2.reflectance = 0.5;
    
    Vector center3(200.0f, 0.0f, -600.0f);
    RGB sphereColor3(0.0f,0.0f,1.0f);
    Sphere sphere3(center3, 100.0f);
    sphere3.color = sphereColor3;
    sphere3.reflectance = 0.5;

    // planes
    Vector planeNormal1(0.0, 1.0, 0.0);
    RGB planeColor1(0,1,0);
    Plane plane1(planeNormal1, 300.0);
    plane1.color = planeColor1;

    Vector planeNormal2(0.0, 0.0, 1.0);
    RGB planeColor2 (1.0,1.0,1.0,1.0);
    Plane plane2(planeNormal2, 700.0);
    plane2.color = planeColor2;

    Vector planeNormal3(0.0, -1.0, 0.0);
    RGB planeColor3(1.0,1.0,1.0,1.0);
    Plane plane3(planeNormal3, 500.0);
    plane3.color = planeColor3;

    Vector planeNormal4(1.0, 0.0, 0.0);
    RGB planeColor4(1.0,0.0,0.0,1.0);
    Plane plane4(planeNormal4, 400.0);
    plane4.color = planeColor4;

    Vector planeNormal5(-1.0, 0.0, 0.0);
    RGB planeColor5(1.0,0.0,1.0,1.0);
    Plane plane5(planeNormal5, 400.0);
    plane5.color = planeColor5;

    // add objects
    Scene myScene;
    myScene.addShape(&sphere1);
    myScene.addShape(&sphere2);
    myScene.addShape(&sphere3);
    myScene.addShape(&plane1);
    myScene.addShape(&plane2);
    myScene.addShape(&plane3);
    myScene.addShape(&plane4);
    myScene.addShape(&plane5);

    // make a 640x480 image
    Image img(640, 480);

    // tracer
    RayTracer tracer(myScene);
    tracer.eye = Vector(0.0f,0.0f,0.0f);
    tracer.light.pos = Vector(300.0f, 400.0f, -100.0f);

    tracer.setImage(&img);
    printf("building...\n");
    tracer.buildGlobalMap(3);
    // tracer.buildCausticsMap(3);
    printf("rendering...\n");
    tracer.render();
    // tracer.renderMap();
    printf("done\n");

    img.WriteTga("myout.tga", true);
}

void debug()
{
    RGB c1(1.0, 0.9, 0.8);
    RGB c2(0.8, 0.9, 0.8, 0.5);
    RGB c3(0.0, 0.0, 0.1, 0.2);
    RGB c4(100.0, -100.0, 0.0, 0.8);

    double t = 0.6;

    std::cout << c1 + c2 << std::endl;
    std::cout << c2 + c3 << std::endl;
    std::cout << c3 + c4 << std::endl;
    std::cout << c4 + c1 << std::endl;
    std::cout << c1 * t << std::endl;
    std::cout << c2 * t << std::endl;
    std::cout << c3 * t << std::endl;
    std::cout << c4 * t << std::endl;
    std::cout << c1 + t << std::endl;
    std::cout << c2 + t << std::endl;
    std::cout << c3 + t << std::endl;
    std::cout << c4 + t << std::endl;
    std::cout << RGB::filter(c1, c2) << std::endl;
    std::cout << RGB::filter(c2, c3) << std::endl;
    std::cout << RGB::filter(c3, c4) << std::endl;
    std::cout << RGB::filter(c4, c1) << std::endl;
    c1.scale();
    c2.scale();
    c3.scale();
    c4.scale();
    std::cout << c1 << std::endl;
    std::cout << c2 << std::endl;
    std::cout << c3 << std::endl;
    std::cout << c4 << std::endl;
}*/

int main(int argc, char** argv)
{
    RayTracer tracer;
    tracer.setup("scenes/balls.json");
    printf("building...\n");
    tracer.buildGlobalMap();
    printf("rendering...\n");
    tracer.render();
     
    return 0;
}