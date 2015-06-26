#ifndef UTIL_SHAPE_OBJECT
#define UTIL_SHAPE_OBJECT

#include "Shape.h"
#include "Triangle.h"
#include "BBox.h"

struct KDTNode
{
    std::vector<int> faces; // indexes of faces
    BBox bbox; // bounding box
    std::shared_ptr<KDTNode> left, right; // children
};

class Object : public Shape
{
public:
    Object(const std::string& file);

    bool hit(const Ray& ray, double time, HitRecord& record) const;

    RGB colorAt(const Vector& pos) const;

    static std::shared_ptr<Shape> parse(const JsonBox::Value& val, 
        const std::string& CLS);

private:
    std::vector<Triangle> _triangles;
    std::shared_ptr<KDTNode> _tree;

    void loadObjFile(const std::string& file);
    std::shared_ptr<KDTNode> build(const std::vector<int>& faces, int depth);
    bool search(std::shared_ptr<KDTNode> node, const Ray& ray, 
        double time, HitRecord record) const;
};

#endif