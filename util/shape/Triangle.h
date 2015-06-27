#ifndef TRIANGLE_H_
#define TRIANGLE_H_

#include "Shape.h"
#include "BBox.h"

/* A triangle defined as

         v2
        / \           
       /   \         
      /     \       
     /       \     
    v1-------v3

 */

class Triangle : public Shape
{
public:
    Triangle(const Vector& v1, const Vector& v2, const Vector& v3)
        : v1(v1), v2(v2), v3(v3), _mid((v1 + v2 + v3) / 3.0) {}

    static std::shared_ptr<Shape> parse(const JsonBox::Value& val,
        const std::string& CLS);

    bool hit(const Ray& ray, double time, HitRecord& record) const;

    RGB colorAt(const Vector& pos) const;

    Vector midPoint() const { return _mid; };

    BBox boundingBox() const;

    friend std::ostream& operator << (std::ostream& os, const Triangle& tri);

    Vector v1;
    Vector v2;
    Vector v3;
    Vector _mid;
};

#endif // TRIANGLE_H_