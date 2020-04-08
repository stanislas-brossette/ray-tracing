#include "geometries/HierarchyBoundingVolume.hh"

Node::Node(const Frame3& f, int depth, int maxDepth)
    : bp_(f, {
        {1,0,0},
        {0,1,0},
        {0,0,1},
        {0.57735026919,0.57735026919,0.57735026919},
        {-0.57735026919,0.57735026919,0.57735026919},
        {0.57735026919,-0.57735026919,0.57735026919},
        {-0.57735026919,-0.57735026919,0.57735026919}}),
    depth_(depth),
    maxDepth_(maxDepth)
{
}

Node::Node(const Frame3& f, int depth, int maxDepth,
        const std::vector<Vector3>& normals,
        const std::vector<double>& pMin,
        const std::vector<double>& pMax)
    : bp_(f, normals, pMin, pMax),
    depth_(depth),
    maxDepth_(maxDepth)
{
}

void Node::spawnChildren()
{
    if(depth_ == maxDepth_)
        return;
    std::vector<double> bp__pMid_(bp_.pMin_.size());
    std::vector<double> eps(bp_.pMin_.size());
    for (size_t i = 0; i < bp_.pMin_.size(); i++)
    {
        bp__pMid_[i] = (bp_.pMin_[i]+bp_.pMax_[i])/2.0;
        eps[i] = std::abs(bp_.pMax_[i]-bp_.pMin_[i])/1000.0;
    }
    std::vector<Vector3> basicNormals{{1,0,0},{0,1,0},{0,0,1}};
    children_.push_back(new Node(bp_.f_, depth_+1, maxDepth_, basicNormals,
                {bp_.pMin_[0] - eps[0], bp_.pMin_[1] - eps[1], bp_.pMin_[2] - eps[2]},
                {bp__pMid_[0] + eps[0], bp__pMid_[1] + eps[1], bp__pMid_[2] + eps[2]}));
    children_.push_back(new Node(bp_.f_, depth_+1, maxDepth_, basicNormals,
                {bp__pMid_[0] - eps[0], bp_.pMin_[1] - eps[1], bp_.pMin_[2] - eps[2]},
                {bp_.pMax_[0] + eps[0], bp__pMid_[1] + eps[1], bp__pMid_[2] + eps[2]}));
    children_.push_back(new Node(bp_.f_, depth_+1, maxDepth_, basicNormals,
                {bp_.pMin_[0] - eps[0], bp__pMid_[1] - eps[1], bp_.pMin_[2] - eps[2]},
                {bp__pMid_[0] + eps[0], bp_.pMax_[1] + eps[1], bp__pMid_[2] + eps[2]}));
    children_.push_back(new Node(bp_.f_, depth_+1, maxDepth_, basicNormals,
                {bp_.pMin_[0] - eps[0], bp_.pMin_[1] - eps[1], bp__pMid_[2] - eps[2]},
                {bp__pMid_[0] + eps[0], bp__pMid_[1] + eps[1], bp_.pMax_[2] + eps[2]}));
    children_.push_back(new Node(bp_.f_, depth_+1, maxDepth_, basicNormals,
                {bp__pMid_[0] - eps[0], bp__pMid_[1] - eps[1], bp_.pMin_[2] - eps[2]},
                {bp_.pMax_[0] + eps[0], bp_.pMax_[1] + eps[1], bp__pMid_[2] + eps[2]}));
    children_.push_back(new Node(bp_.f_, depth_+1, maxDepth_, basicNormals,
                {bp__pMid_[0] - eps[0], bp_.pMin_[1] - eps[1], bp__pMid_[2] - eps[2]},
                {bp_.pMax_[0] + eps[0], bp__pMid_[1] + eps[1], bp_.pMax_[2] + eps[2]}));
    children_.push_back(new Node(bp_.f_, depth_+1, maxDepth_, basicNormals,
                {bp_.pMin_[0] - eps[0], bp__pMid_[1] - eps[1], bp__pMid_[2] - eps[2]},
                {bp__pMid_[0] + eps[0], bp_.pMax_[1] + eps[1], bp_.pMax_[2] + eps[2]}));
    children_.push_back(new Node(bp_.f_, depth_+1, maxDepth_, basicNormals,
                {bp__pMid_[0] - eps[0], bp__pMid_[1] - eps[1], bp__pMid_[2] - eps[2]},
                {bp_.pMax_[0] + eps[0], bp_.pMax_[1] + eps[1], bp_.pMax_[2] + eps[2]}));

    for (size_t i = 0; i < children_.size(); i++)
    {
        children_[i]->spawnChildren();
    }
}

bool Node::testIntersectionWithEdge(const Vector3& p0, const Vector3& p1)
{
    Vector3 edge = p1 - p0;
    double edgeLength = edge.norm();
    Vector3 edgeDir = edge.normalize();
    LightRay lrEdge(p0, edgeDir);

    Vector3 point;
    Vector3 normal;
    double dist;
    bool intersection = bp_.intersect(lrEdge, point, normal, dist);
    bool res = intersection and dist >= 0 and dist <= edgeLength;
    return res;
}

void Node::populateChildren(const Vector3& p0, const Vector3& p1, const Vector3& p2, int index)
{
    // By construction, the depth0 bounding volume is guaranteed to contain all the points of the mesh.
    // First, check if at least one vertex of the triangle is inside the boundingPolyhedron
    if(depth_ == 0 or bp_.contains(p0) or bp_.contains(p1) or bp_.contains(p2))
    {
        includedIndices_.push_back(index);
        for (size_t i = 0; i < children_.size(); i++)
        {
            children_[i]->populateChildren(p0, p1, p2, index);
        }
        return;
    }

    // Second, check if any edge of the triangle intersectes the boundingPolyhedron
    if(testIntersectionWithEdge(p0, p1) or
        testIntersectionWithEdge(p1, p2) or
        testIntersectionWithEdge(p2, p0))
    {
        includedIndices_.push_back(index);
        for (size_t i = 0; i < children_.size(); i++)
        {
            children_[i]->populateChildren(p0, p1, p2, index);
        }
        return;
    }

    // Third, check if any vertex of the boundingPolyhedron projected onto the triangle plane
    // is inside the triangle
}


bool Node::intersect(const LightRay& incident, std::vector<NodeIntersection>& nodeIntersections) const
{
    if(includedIndices_.size() == 0)
        return false;
    }

    Vector3 point(0,0,0);
    Vector3 normal(0,0,0);
    double dist = 0;

    if(bp_.intersect(incident, point, normal, dist))
    {
        if(depth_ == maxDepth_)
        {
            nodeIntersections.push_back(NodeIntersection(point, normal, dist, this));
            return true;
        }
        else
        {
            bool intersectWithAnyChild = false;
            for (size_t i = 0; i < children_.size(); i++)
            {
                bool childIntersect = children_[i]->intersect(incident, nodeIntersections);
                intersectWithAnyChild = intersectWithAnyChild or childIntersect;
            }
            return intersectWithAnyChild;
        }
    }
    else
        return false;
}

std::string Node::describe() const
{
    std::stringstream ss;
    ss << "Node depth: " << depth_ << "\n";
    ss << bp_.describe();
    ss << "nTriangles: " << includedIndices_.size() << "\n";
    for (size_t i = 0; i < children_.size(); i++)
    {
        ss << children_[i]->describe();
    }
    return ss.str();
}

HierarchyBoundingVolume::HierarchyBoundingVolume(const Frame3& f, int maxDepth)
    : f_(f),
    root_(nullptr),
    maxDepth_(maxDepth)
{
    int depth = 0;
    root_ = new Node(f_, depth, maxDepth_);
}

bool HierarchyBoundingVolume::intersect(const LightRay& incident, Vector3& point, Vector3& normal, double& dist, std::vector<NodeIntersection>& nodeIntersections) const
{
    bool intersection = root_->intersect(incident, nodeIntersections);
    return intersection;
}

void HierarchyBoundingVolume::extendBy(const Vector3& point)
{
    root_->bp_.extendBy(point);
}

void HierarchyBoundingVolume::extendByTriangle(const Vector3& p0, const Vector3& p1, const Vector3& p2, int index)
{
    root_->bp_.extendBy(p0);
    root_->bp_.extendBy(p1);
    root_->bp_.extendBy(p2);
}

void HierarchyBoundingVolume::finishFirstPass()
{
    root_->spawnChildren();
}

void HierarchyBoundingVolume::populateWithTriangle(const Vector3& p0, const Vector3& p1, const Vector3& p2, int index)
{
    root_->populateChildren( p0, p1, p2, index);
}

std::string HierarchyBoundingVolume::describe() const
{
    std::stringstream ss;
    ss << "HierarchyBoundingVolume\n";
    ss << root_->describe();
    return ss.str();
}

