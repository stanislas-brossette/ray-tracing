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
    std::vector<double> pMid(bp_.pMin_.size());
    for (size_t i = 0; i < bp_.pMin_.size(); i++)
    {
        pMid[i] = (bp_.pMin_[i]+bp_.pMax_[i])/2.0;
    }
    std::vector<Vector3> basicNormals{{1,0,0},{0,1,0},{0,0,1}};
    children_.push_back(new Node(bp_.f_, depth_+1, maxDepth_, basicNormals,
                {bp_.pMin_[0], bp_.pMin_[1], bp_.pMin_[2]},
                {pMid[0], pMid[1], pMid[2]}));
    children_.push_back(new Node(bp_.f_, depth_+1, maxDepth_, basicNormals,
                {pMid[0], bp_.pMin_[1], bp_.pMin_[2]},
                {bp_.pMax_[0], pMid[1], pMid[2]}));
    children_.push_back(new Node(bp_.f_, depth_+1, maxDepth_, basicNormals,
                {bp_.pMin_[0], pMid[1], bp_.pMin_[2]},
                {pMid[0], bp_.pMax_[1], pMid[2]}));
    children_.push_back(new Node(bp_.f_, depth_+1, maxDepth_, basicNormals,
                {bp_.pMin_[0], bp_.pMin_[1], pMid[2]},
                {pMid[0], pMid[1], bp_.pMax_[2]}));
    children_.push_back(new Node(bp_.f_, depth_+1, maxDepth_, basicNormals,
                {pMid[0], pMid[1], bp_.pMin_[2]},
                {bp_.pMax_[0], bp_.pMax_[1], pMid[2]}));
    children_.push_back(new Node(bp_.f_, depth_+1, maxDepth_, basicNormals,
                {pMid[0], bp_.pMin_[1], pMid[2]},
                {bp_.pMax_[0], pMid[1], bp_.pMax_[2]}));
    children_.push_back(new Node(bp_.f_, depth_+1, maxDepth_, basicNormals,
                {bp_.pMin_[0], pMid[1], pMid[2]},
                {pMid[0], bp_.pMax_[1], bp_.pMax_[2]}));
    children_.push_back(new Node(bp_.f_, depth_+1, maxDepth_, basicNormals,
                {pMid[0], pMid[1], pMid[2]},
                {bp_.pMax_[0], bp_.pMax_[1], bp_.pMax_[2]}));

    for (size_t i = 0; i < children_.size(); i++)
    {
        children_[i]->spawnChildren();
    }
}

void Node::populateChildren(const Vector3& p0, const Vector3& p1, const Vector3& p2, int index)
{
    // By construction, the depth0 bounding volume is guaranteed to contain all the points of the mesh.
    if(depth_ == 0 or bp_.contains(p0) or bp_.contains(p1) or bp_.contains(p2))
    {
        includedIndices_.push_back(index);
        for (size_t i = 0; i < children_.size(); i++)
        {
            children_[i]->populateChildren(p0, p1, p2, index);
        }
    }
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
    std::cout << "ctor HBV" << std::endl;
    int depth = 0;
    root_ = new Node(f_, depth, maxDepth_);
}

bool HierarchyBoundingVolume::intersect(const LightRay& incident, Vector3& point, Vector3& normal, double& dist) const
{
    return root_->bp_.intersect(incident, point, normal, dist);
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

