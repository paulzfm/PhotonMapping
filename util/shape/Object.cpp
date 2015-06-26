#include "Object.h"
#include "../../third-party/objparser/SimpleObject.h"

#include <limits>

Object::Object(const std::string& file)
{
    loadObjFile(file);
    std::vector<int> faces;
    for (int i = 0; i < _triangles.size(); ++i) {
        faces.push_back(i);
    }
    _tree = build(faces, 0);
}

bool Object::hit(const Ray& ray, double time, HitRecord& record) const
{
    return search(_tree, ray, time, record);
}

RGB Object::colorAt(const Vector& pos) const
{
    return color;
}

void Object::loadObjFile(const std::string& file)
{
    std::cout << "--> Loading object: " << file << "\n";

    SimpleOBJ::CSimpleObject obj;
    obj.LoadFromObj(file.c_str());
    for (int i = 0; i < obj.m_nTriangles; ++i) {
        SimpleOBJ::Array<int, 3> t = obj.m_pTriangleList[i];
        _triangles.push_back(Triangle(
            Vector(obj.m_pVertexList[t[0]].x, obj.m_pVertexList[t[0]].y,
                   obj.m_pVertexList[t[0]].z),
            Vector(obj.m_pVertexList[t[1]].x, obj.m_pVertexList[t[1]].y,
                   obj.m_pVertexList[t[1]].z),
            Vector(obj.m_pVertexList[t[2]].x ,obj.m_pVertexList[t[2]].y, 
                   obj.m_pVertexList[t[2]].x)
        ));
    }
}

std::shared_ptr<KDTNode> Object::build(const std::vector<int>& faces, int depth)
{
    std::shared_ptr<KDTNode> node = std::shared_ptr<KDTNode>(new KDTNode);
    node->faces = faces;
    node->left = nullptr;
    node->right = nullptr;

    // empty node
    if (faces.size() == 0) {
        return node;
    }

    // tiny node
    if (faces.size() == 1) {
        node->bbox = _triangles[faces[0]].boundingBox();
        node->left = std::shared_ptr<KDTNode>(new KDTNode);
        node->right = std::shared_ptr<KDTNode>(new KDTNode);
        node->left->faces = std::vector<int>();
        node->right->faces = std::vector<int>();
        return node;
    }

    // create bbox containing all faces and find midpoint of
    // all triangles
    Vector mid;
    for (const auto& f : faces) {
        node->bbox.expand(_triangles[f].boundingBox());
        mid += _triangles[f].midPoint();
    }
    mid /= (double)faces.size();

    // split faces into two parts
    std::vector<int> left_faces, right_faces;
    int axis = node->bbox.longestAxis();
    switch (axis) {
    case BBox::X:
        for (const auto& f : faces) {
            mid.x >= _triangles[f].midPoint().x ? right_faces.push_back(f)
                : left_faces.push_back(f);
        }
        break;
    case BBox::Y:
        for (const auto& f : faces) {
            mid.y >= _triangles[f].midPoint().y ? right_faces.push_back(f)
                : left_faces.push_back(f);
        }
        break;
    case BBox::Z:
        for (const auto& f : faces) {
            mid.z >= _triangles[f].midPoint().z ? right_faces.push_back(f)
                : left_faces.push_back(f);
        }
        break;
    }


    if (left_faces.size() == 0 && right_faces.size() > 0) {
        left_faces = right_faces;
    }
    if (right_faces.size() == 0 && left_faces.size() > 0) {
        right_faces = left_faces;
    }

    // search how many faces are the same
    int same = 0;
    for (const auto& f1 : left_faces) {
        for (const auto& f2 : right_faces) {
            if (f1 == f2) {
                ++same;
            }
        }
    }

    // if >=50% are the same, stop dividing
    if ((double)same / left_faces.size() >= 0.5 
        && (double)same / right_faces.size() >= 0.5) {
        node->left = std::shared_ptr<KDTNode>(new KDTNode);
        node->right = std::shared_ptr<KDTNode>(new KDTNode);
        node->left->faces = std::vector<int>();
        node->right->faces = std::vector<int>();
        return node;
    }

    // recursively build left and right child
    node->left = build(left_faces, depth + 1);
    node->right = build(right_faces, depth + 1);

    return node;
}

bool Object::search(std::shared_ptr<KDTNode> node, const Ray& ray, 
    double time, HitRecord record) const
{
    if (node->bbox.shadowHit(ray)) {
        if (node->left->faces.size() > 0 || node->right->faces.size()) {
            return search(node, ray, time, record)
                || search(node, ray, time, record);

        } else { // leaf node
            double max_dis = std::numeric_limits<double>::max();
            bool hit = false;

            // hit with each face
            for (const auto& f : node->faces) {
                if (_triangles[f].hit(ray, time, record)) {
                    Vector v = ray.o + record.t * ray.d;
                    double dis = (v - ray.o).square();
                    if (dis < max_dis) {
                        hit = true;
                        max_dis = dis;
                    }
                }
            }

            return hit;
        }
    }

    return false;
}

