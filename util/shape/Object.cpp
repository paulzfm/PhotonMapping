#include "Object.h"
#include "../../third-party/objparser/SimpleObject.h"

#include <limits>
#include <assert.h>

std::ostream& operator << (std::ostream& os, std::shared_ptr<KDTNode> node)
{
    os << node->bbox << " ";
    for (const auto f : node->faces) {
        os << f << " ";
    }

    return os;
}

void KDTNode::print(std::shared_ptr<KDTNode> node, int indent)
{
    if (!node) {
        return;
    }

    for (int i = 0; i < indent; ++i) {
        printf(" ");
    }
    std::cout << node << std::endl;

    print(node->left, indent + 4);
    print(node->right, indent + 4);
}

Object::Object(const std::string& file, const Vector& a, const Vector& b)
    : _min(a), _max(b)
{
    loadObjFile(file);
    std::vector<int> faces;
    for (int i = 0; i < _triangles.size(); ++i) {
        faces.push_back(i);
    }
    _tree = build(faces, 0);

    name = "object";

    printTree();
}

void Object::printTree()
{
    KDTNode::print(_tree);
}

bool Object::hit(const Ray& ray, double time, HitRecord& record) const
{
    // bool ret = search(_tree, ray, time, record);
    // if (ret) {
        // std::cout << "hit at " << record.n << std::endl;
    // } else {
        // std::cout << "failed!" << std::endl;
    // }

    /* naive method */
    double max_dis = std::numeric_limits<double>::max();
    bool res = false;
    int face = -1;

    // hit with each face
    for (const auto& f : _tree->faces) {
        HitRecord tmp;
        if (_triangles[f].hit(ray, time, tmp)) {
            double dis = (-tmp.t * ray.d).square();
            if (dis < max_dis) {
                record = tmp;
                res = true;
                face = f;
                max_dis = dis;
            }
        }
    }

    return res;
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
    Vector v_min(std::numeric_limits<double>::max(),
                 std::numeric_limits<double>::max(),
                 std::numeric_limits<double>::max());
    Vector v_max(-std::numeric_limits<double>::max(),
                 -std::numeric_limits<double>::max(),
                 -std::numeric_limits<double>::max());

    // load vertices
    for (int i = 0; i < obj.m_nVertices; ++i) {
        Vector v(obj.m_pVertexList[i].x, obj.m_pVertexList[i].y, 
            obj.m_pVertexList[i].z);
        _vertices.push_back(v);
        
        v_min.x = MIN(v_min.x, v.x);
        v_min.y = MIN(v_min.y, v.y);
        v_min.z = MIN(v_min.z, v.z);
        v_max.x = MAX(v_max.x, v.x);
        v_max.y = MAX(v_max.y, v.y);
        v_max.z = MAX(v_max.z, v.z);
    }

    printf("all vertices before scale:\n");
    for (const auto& v : _vertices) {
        std::cout << v << std::endl;
    }

    // scale
    Vector v_scale = Vector(
        (_max - _min).x / (v_max - v_min).x,
        (_max - _min).y / (v_max - v_min).y,
        (_max - _min).z / (v_max - v_min).z
    );
    std::cout << "scalev:" << v_scale << std::endl;
    double scale = MIN(v_scale.x, MIN(v_scale.y, v_scale.z));

    std::cout << v_min << "VS" << _min << ":" << scale << std::endl;
    for (auto& v : _vertices) {
        v = (v - v_min) * scale + _min;
    }

    printf("all vertices after scale:\n");
    int cnt = 0;
    for (const auto& v : _vertices) {
        std::cout << cnt++ << " - " << v << std::endl;
    }

    // load faces
    for (int i = 0; i < obj.m_nTriangles; ++i) {
        SimpleOBJ::Array<int, 3> t = obj.m_pTriangleList[i];
        _triangles.push_back(Triangle(_vertices[t[0]], _vertices[t[1]],
            _vertices[t[2]]));
    }

    printf("all faces range after scale:\n");
    cnt = 0;
    for (const auto& t : _triangles) {
        std::cout << cnt++ << " - " << t.boundingBox() << std::endl;
    }
}

// refer to:
//     http://blog.frogslayer.com/kd-trees-for-faster-ray-tracing-with-triangles/
std::shared_ptr<KDTNode> Object::build(const std::vector<int>& faces, int depth)
{
    std::shared_ptr<KDTNode> node = std::shared_ptr<KDTNode>(new KDTNode);
    node->faces = faces;

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
    node->bbox = _triangles[faces[0]].boundingBox();

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
    default:
        fprintf(stderr, "Error: invalid axis type: %d\n", axis);
        exit(1);
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
    if ((double)same / left_faces.size() >= 0.5 &&
        (double)same / right_faces.size() >= 0.5) {
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
    double time, HitRecord& record) const
{
    if (node->bbox.shadowHit(ray)) {
        if (node->left->faces.size() > 0 || node->right->faces.size() > 0) {
            return search(node->left, ray, time, record)
                || search(node->right, ray, time, record);

        } else { // leaf node
            double max_dis = std::numeric_limits<double>::max();
            bool hit = false;
            int face = -1;

            // hit with each face
            for (const auto& f : node->faces) {
                HitRecord tmp;
                if (_triangles[f].hit(ray, time, tmp)) {
                    double dis = (-tmp.t * ray.d).square();
                    if (dis < max_dis) {
                        record = tmp;
                        hit = true;
                        face = f;
                        max_dis = dis;
                    }
                }
            }

            return hit;
        }
    }

    return false;
}

std::shared_ptr<Shape> Object::parse(const JsonBox::Value& val, 
    const std::string& CLS)
{
    Parser::checkObject(val, CLS);

    JsonBox::Object obj = val.getObject();
    Parser::checkParam(obj, CLS, "model", Parser::STRING);
    Parser::checkParam(obj, CLS, "A", Parser::VEC3);
    Parser::checkParam(obj, CLS, "B", Parser::VEC3);

    return std::shared_ptr<Shape>(new Object(
        Parser::asString(obj["model"]),
        Parser::asVector(obj["A"]),
        Parser::asVector(obj["B"])
    ));
}


