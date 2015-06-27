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

    friend std::ostream& operator << (std::ostream& os, std::shared_ptr<KDTNode> node);
    static void print(std::shared_ptr<KDTNode> node, int indent = 0); // recursively print
};

class Object : public Shape
{
public:
    Object(const std::string& file, const Vector& a, const Vector& b);

    bool hit(const Ray& ray, double time, HitRecord& record) const;
    
    bool slowHit(const Ray& ray, double time, HitRecord& record) const;

    RGB colorAt(const Vector& pos) const;

    static std::shared_ptr<Shape> parse(const JsonBox::Value& val, 
        const std::string& CLS);

private:
    std::vector<Vector> _vertices;
    std::vector<Triangle> _triangles;
    std::shared_ptr<KDTNode> _tree;
    Vector _min, _max; // left-bottom and right-top

    void loadObjFile(const std::string& file);
    std::shared_ptr<KDTNode> build(const std::vector<int>& faces, int depth);
    bool search(std::shared_ptr<KDTNode> node, const Ray& ray, 
        double time, HitRecord& record) const;
    
    // debug ONLY
    void printTree();
};

#endif