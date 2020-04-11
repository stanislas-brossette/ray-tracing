#pragma once
#include <iostream>
#include <vector>
#include <string>
#include <sstream>
#include <utility>

#include "Frame3.hh"
#include "Vector3.hh"
#include "BoundingPolyhedron.hh"
#include "LightRay.hh"

class Node;

struct NodeIntersection
{
    NodeIntersection(const Vector3& point, const Vector3& normal, double dist, const Node* node)
        : point_(point),
        normal_(normal),
        dist_(dist),
        node_(node)
    {
    }

    Vector3 point_;
    Vector3 normal_;
    double dist_;
    const Node* node_;
};

class Node
{
public:
    Node (const Frame3& f, int depth, int maxDepth, const std::string& name);
    Node(const Frame3& f, int depth, int maxDepth,
            const std::vector<Vector3>& normals,
            const std::vector<double>& pMin,
            const std::vector<double>& pMax,
            const std::string& name);
    ~Node () = default;
    void spawnChildren();
    void populateChildren(const Vector3& p0, const Vector3& p1, const Vector3& p2, int index);
    bool intersect(const LightRay& incident, std::vector<NodeIntersection>& nodeIntersections) const;
    bool testIntersectionWithEdge(const Vector3& p0, const Vector3& p1);

    std::string describe() const;

    BoundingPolyhedron bp_;
    std::vector<Node*> children_;
    std::vector<int> includedIndices_;
    int depth_;
    int maxDepth_;
    std::string name_;
};

class HierarchyBoundingVolume
{
public:
    HierarchyBoundingVolume () = default;
    HierarchyBoundingVolume (const Frame3& f, int maxDepth = 5);
    ~HierarchyBoundingVolume () = default;
    bool intersect(const LightRay& incident, Vector3& point, Vector3& normal, double& dist, std::vector<NodeIntersection>& nodeIntersection) const;
    void extendBy(const Vector3& point);
    void extendByTriangle(const Vector3& p0, const Vector3& p1, const Vector3& p2, int index);
    void extendByPolygon(const std::vector<Vector3>& points);
    std::string describe() const;
    void finishFirstPass();
    void populateWithTriangle(const Vector3& p0, const Vector3& p1, const Vector3& p2, int index);

    int maxDepth_;
private:
    Frame3 f_;
    Node* root_;
};
