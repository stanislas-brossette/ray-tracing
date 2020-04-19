#include "geometries/HierarchyBoundingVolume.hh"

Node::Node(const Frame3& f, int depth, int maxDepth, const std::string& name)
    : bp_(f, {
        {1,0,0},
        {0,1,0},
        {0,0,1},
        {0.57735026919,0.57735026919,0.57735026919},
        {-0.57735026919,0.57735026919,0.57735026919},
        {0.57735026919,-0.57735026919,0.57735026919},
        {-0.57735026919,-0.57735026919,0.57735026919}}),
    depth_(depth),
    maxDepth_(maxDepth),
    name_(name)
{
}

Node::Node(const Frame3& f, int depth, int maxDepth,
        const std::vector<Vector3>& normals,
        const std::vector<double>& pMin,
        const std::vector<double>& pMax,
        const std::string& name)
    : bp_(f, normals, pMin, pMax),
    depth_(depth),
    maxDepth_(maxDepth),
    name_(name)
{
}

void Node::spawnChildren()
{
    bp_.finalize();
    if(depth_ == maxDepth_)
        return;
    std::vector<double> bp__pMid_(bp_.pMin_.size());
    std::vector<double> eps(bp_.pMin_.size());
    for (size_t i = 0; i < bp_.pMin_.size(); i++)
    {
        bp__pMid_[i] = (bp_.pMin_[i]+bp_.pMax_[i])/2.0;
        eps[i] = 1e-6*std::abs(bp_.pMax_[i]-bp_.pMin_[i]);
    }
    std::vector<Vector3> basicNormals{{1,0,0},{0,1,0},{0,0,1}};
    children_.push_back(new Node(bp_.f_, depth_+1, maxDepth_, basicNormals,
                {bp_.pMin_[0] - eps[0], bp_.pMin_[1] - eps[1], bp_.pMin_[2] - eps[2]},
                {bp__pMid_[0] + eps[0], bp__pMid_[1] + eps[1], bp__pMid_[2] + eps[2]},
                name_ + std::to_string(0)));
    children_.push_back(new Node(bp_.f_, depth_+1, maxDepth_, basicNormals,
                {bp__pMid_[0] - eps[0], bp_.pMin_[1] - eps[1], bp_.pMin_[2] - eps[2]},
                {bp_.pMax_[0] + eps[0], bp__pMid_[1] + eps[1], bp__pMid_[2] + eps[2]},
                name_ + std::to_string(1)));
    children_.push_back(new Node(bp_.f_, depth_+1, maxDepth_, basicNormals,
                {bp_.pMin_[0] - eps[0], bp__pMid_[1] - eps[1], bp_.pMin_[2] - eps[2]},
                {bp__pMid_[0] + eps[0], bp_.pMax_[1] + eps[1], bp__pMid_[2] + eps[2]},
                name_ + std::to_string(2)));
    children_.push_back(new Node(bp_.f_, depth_+1, maxDepth_, basicNormals,
                {bp_.pMin_[0] - eps[0], bp_.pMin_[1] - eps[1], bp__pMid_[2] - eps[2]},
                {bp__pMid_[0] + eps[0], bp__pMid_[1] + eps[1], bp_.pMax_[2] + eps[2]},
                name_ + std::to_string(3)));
    children_.push_back(new Node(bp_.f_, depth_+1, maxDepth_, basicNormals,
                {bp__pMid_[0] - eps[0], bp__pMid_[1] - eps[1], bp_.pMin_[2] - eps[2]},
                {bp_.pMax_[0] + eps[0], bp_.pMax_[1] + eps[1], bp__pMid_[2] + eps[2]},
                name_ + std::to_string(4)));
    children_.push_back(new Node(bp_.f_, depth_+1, maxDepth_, basicNormals,
                {bp__pMid_[0] - eps[0], bp_.pMin_[1] - eps[1], bp__pMid_[2] - eps[2]},
                {bp_.pMax_[0] + eps[0], bp__pMid_[1] + eps[1], bp_.pMax_[2] + eps[2]},
                name_ + std::to_string(5)));
    children_.push_back(new Node(bp_.f_, depth_+1, maxDepth_, basicNormals,
                {bp_.pMin_[0] - eps[0], bp__pMid_[1] - eps[1], bp__pMid_[2] - eps[2]},
                {bp__pMid_[0] + eps[0], bp_.pMax_[1] + eps[1], bp_.pMax_[2] + eps[2]},
                name_ + std::to_string(6)));
    children_.push_back(new Node(bp_.f_, depth_+1, maxDepth_, basicNormals,
                {bp__pMid_[0] - eps[0], bp__pMid_[1] - eps[1], bp__pMid_[2] - eps[2]},
                {bp_.pMax_[0] + eps[0], bp_.pMax_[1] + eps[1], bp_.pMax_[2] + eps[2]},
                name_ + std::to_string(7)));

    for (size_t i = 0; i < children_.size(); i++)
    {
        children_[i]->spawnChildren();
    }
}

bool Node::testIntersectionWithEdge(const Vector3& p0, const Vector3& p1, bool verbose)
{
    Vector3 edge = p1 - p0;
    double edgeLength = edge.norm();
    Vector3 edgeDir = edge.normalize();
    LightRay lrEdge(p0, edgeDir);

    Vector3 point;
    Vector3 normal;
    double dist;
    bool intersection = bp_.intersect(lrEdge, point, normal, dist, verbose);
    bool res = intersection and dist >= 0 and dist <= edgeLength;
    return res;
}

void Node::populateChildren(const std::vector<Vector3>& points, int index)
{
    // By construction, the depth0 bounding volume is guaranteed to contain all the points of the mesh.
    // First, check if at least one vertex of the triangle is inside the boundingPolyhedron
    bool containsPoint = false;
    if(depth_ == 0)
        containsPoint = true;
    else
    {
        for (size_t i = 0; i < points.size(); i++)
        {
            if(bp_.contains(points[i]))
            {
                containsPoint = true;
                break;
            }
        }
    }
    if(containsPoint)
    {
        includedIndices_.push_back(index);
        for (size_t i = 0; i < children_.size(); i++)
        {
            children_[i]->populateChildren(points, index);
        }
        return;
    }

    // Second, check if any edge of the triangle intersectes the boundingPolyhedron
    bool intersectWithEdge = false;
    for (size_t i = 0; i < points.size(); i++)
    {
        if(testIntersectionWithEdge(points[i], points[(i+1)%points.size()]))
        {
            intersectWithEdge = true;
            break;
        }
    }
    if(intersectWithEdge)
    {
        includedIndices_.push_back(index);
        for (size_t i = 0; i < children_.size(); i++)
        {
            children_[i]->populateChildren(points, index);
        }
        return;
    }

    // Third, check if any vertex of the boundingPolyhedron projected onto the triangle plane
    // is inside the triangle
    {
        Vector3 triO = points[0];
        Vector3 triX = (points[1]-points[0]).normalize();
        Vector3 triZ = (triX.cross(points[2]-points[0])).normalize();
        Vector3 triY = triZ.cross(triX);
        //check if there are points above and below triangle plane
        bool pointAbove = false;
        bool pointBelow = false;
        for (size_t i = 0; i < bp_.verticesCube().size(); i++)
        {
            double dot = triZ.dot(bp_.verticesCube().at(i) - triO);
            if(not pointAbove)
                pointAbove = dot>=0;
            if(not pointBelow)
                pointBelow = dot<=0;
            if(pointAbove and pointBelow)
                break;
        }
        if(not(pointAbove and pointBelow))
        {
            return;
        }


        //check if at least one point of the bp projects inside the triangle
        std::vector<Vector2> pP;
        for (size_t i = 0; i < points.size(); i++)
        {
            pP.push_back({(points[i]-triO).dot(triX), (points[i]-triO).dot(triY)});
        }
        for (size_t i = 0; i < bp_.verticesCube().size(); i++)
        {
            Vector3 p = bp_.verticesCube().at(i) - triO;
            Vector2 pProj(p.dot(triX), p.dot(triY));
            bool onSameSideOfAllEdges = true;
            bool rightOfp0p1 = pProj.isRightOf(pP[0], pP[1]);
            for (size_t j = 0; j < pP.size(); j++)
            {
                bool rightOf = pProj.isRightOf(pP[j], pP[(j+1)%pP.size()]);
                if(not(rightOf == rightOfp0p1))
                {
                    onSameSideOfAllEdges = false;
                    break;
                }
            }
            if(onSameSideOfAllEdges)
            {
                includedIndices_.push_back(index);
                for (size_t i = 0; i < children_.size(); i++)
                {
                    children_[i]->populateChildren(points, index);
                }
                return;
            }
        }
    }
    return;
}


bool Node::intersect(const LightRay& incident, std::vector<NodeIntersection>& nodeIntersections, bool verbose) const
{
    if(includedIndices_.size() == 0)
        return false;

    Vector3 point(0,0,0);
    Vector3 normal(0,0,0);
    double dist = 0;

    if(bp_.intersect(incident, point, normal, dist, verbose))
    {
        if(depth_ == maxDepth_)
        {
            // Adding point and normal in nodeIntersection, they are in the item's frame
            nodeIntersections.push_back(NodeIntersection(point, normal, dist, this));
            return true;
        }
        else
        {
            bool intersectWithAnyChild = false;
            for (size_t i = 0; i < children_.size(); i++)
            {
                bool childIntersect = children_[i]->intersect(incident, nodeIntersections, verbose);
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
    ss << "name: " << name_ << "\n";
    ss << bp_.describe();
    ss << "nTriangles: " << includedIndices_.size() << "\n";
    for (size_t i = 0; i < includedIndices_.size(); i++)
    {
        ss << includedIndices_[i] << ", ";
    }
    ss << "\n";
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
    root_ = new Node(f_, depth, maxDepth_, std::to_string(0));
}

bool HierarchyBoundingVolume::intersect(const LightRay& incident, Vector3& point, Vector3& normal, double& dist, std::vector<NodeIntersection>& nodeIntersections, bool verbose) const
{
    bool intersection = root_->intersect(incident, nodeIntersections, verbose);
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

void HierarchyBoundingVolume::extendByPolygon(const std::vector<Vector3>& points)
{
    for (size_t i = 0; i < points.size(); i++)
    {
        root_->bp_.extendBy(points.at(i));
    }
}

void HierarchyBoundingVolume::finishFirstPass()
{
    root_->spawnChildren();
}

void HierarchyBoundingVolume::populateWithPolygon(const std::vector<Vector3>& points, int index)
{
    root_->populateChildren(points, index);
}

std::string HierarchyBoundingVolume::describe() const
{
    std::stringstream ss;
    ss << "HierarchyBoundingVolume\n";
    ss << "HBV depth: " << maxDepth_ << "\n";
    ss << root_->describe();
    return ss.str();
}

