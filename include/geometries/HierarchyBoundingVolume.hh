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


class Node
{
public:
    Node (const Frame3& f, int depth, int maxDepth);
    Node(const Frame3& f, int depth, int maxDepth,
            const std::vector<Vector3>& normals,
            const std::vector<double>& pMin,
            const std::vector<double>& pMax);
    ~Node () = default;
    void spawnChildren();
    void populateChildren(const Vector3& p0, const Vector3& p1, const Vector3& p2, int index);
    bool intersect(const LightRay& incident, Vector3& point, Vector3& normal, double& dist, std::vector<std::pair<double, const Node*> >& intersectingNodes) const;

    std::string describe() const;

    BoundingPolyhedron bp_;
    std::vector<Node*> children_;
    std::vector<int> includedIndices_;
    int depth_;
    int maxDepth_;
};

class HierarchyBoundingVolume
{
public:
    HierarchyBoundingVolume () = default;
    HierarchyBoundingVolume (const Frame3& f, int maxDepth = 1);
    ~HierarchyBoundingVolume () = default;
    bool intersect(const LightRay& incident, Vector3& point, Vector3& normal, double& dist, std::vector<std::pair<double, const Node*> >& intersectingNodes) const;
    void extendBy(const Vector3& point);
    void extendByTriangle(const Vector3& p0, const Vector3& p1, const Vector3& p2, int index);
    std::string describe() const;
    void finishFirstPass();
    void populateWithTriangle(const Vector3& p0, const Vector3& p1, const Vector3& p2, int index);

private:
    Frame3 f_;
    Node* root_;
    int maxDepth_;
};
