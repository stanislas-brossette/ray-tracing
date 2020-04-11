#include "geometries/Mesh.hh"

Mesh::Mesh()
    : Geometry(),
    polygons_(),
    path_(),
    hbv_()
{
}

Mesh::Mesh (const Frame3& f, std::string path)
    : Geometry(f),
    polygons_(),
    path_(path),
    hbv_(f)
{
    readPath();
    initTriangles();
}

Mesh::Mesh (MeshData* mData)
    : Geometry(mData),
    polygons_(),
    path_(mData->path),
    hbv_(f_, mData->depthHBV)
{
    readPath();
    initTriangles();
}

Mesh::~Mesh ()
{
}

void Mesh::readPath()
{
    if(not fileExists(path_))
        path_ = std::string(MESHES) + path_;
    if(not fileExists(path_))
        std::cout << "ERROR, mesh file does not exist: " << path_ << std::endl;

    std::string extension = path_.substr(path_.size() - 3);
    if(extension.compare("obj") == 0)
        meshType_ = MeshType::obj;
    else if(extension.compare("stl") == 0)
        meshType_ = MeshType::stl;
    else
        std::cout << "ERROR, mesh file does not exist" << std::endl;
}

void Mesh::initTriangles()
{
    std::vector<float> coords, normals;
    std::vector<unsigned int> tris, solids;
    stl_reader::ReadStlFile (path_.c_str(), coords, normals, tris, solids);

    size_t numTris = tris.size() / 3;
    polygons_.resize(numTris);
    for(size_t itri = 0; itri < numTris; ++itri)
    {
        float* n = &normals [3 * itri];

        float* c = &coords[3 * tris [3 * itri]];
        Vector3 P0(c[0], c[1], c[2]);
        c = &coords[3 * tris [3 * itri + 1]];
        Vector3 P1(c[0], c[1], c[2]);
        c = &coords[3 * tris [3 * itri + 2]];
        Vector3 P2(c[0], c[1], c[2]);

        hbv_.extendByTriangle(P0, P1, P2, itri);

        Vector3 vz(n[0], n[1], n[2]);
        vz.normalize();
        Vector3 vx = (P1 - P0).normalize();
        Vector3 vy = vz.cross(vx);
        Frame3 tFrame(f_.o_ + P0, vx, vy, vz);
        std::vector<Vector2> points(3);
        points[0] = Vector2(0,0);
        points[1] = Vector2((P2 - P0).dot(vx), (P2 - P0).dot(vy));
        points[2] = Vector2((P1 - P0).dot(vx), (P1 - P0).dot(vy));
        polygons_[itri] = Polygon(tFrame, points);
    }
    hbv_.finishFirstPass();
    for(size_t itri = 0; itri < numTris; ++itri)
    {
        float* c = &coords[3 * tris [3 * itri]];
        Vector3 P0(c[0], c[1], c[2]);
        c = &coords[3 * tris [3 * itri + 1]];
        Vector3 P1(c[0], c[1], c[2]);
        c = &coords[3 * tris [3 * itri + 2]];
        Vector3 P2(c[0], c[1], c[2]);
        hbv_.populateWithTriangle(P0, P1, P2, itri);
    }
}

std::string Mesh::describe() const
{
    std::stringstream ss;
    ss << "=== Mesh ===\n";
    ss << "origin: " << f_.o_ << "\n";
    ss << "nTri: " << polygons_.size() << "\n";
    ss << "depthHBV: " << hbv_.maxDepth_ << "\n";
    ss << "meshType: " << to_string(meshType_) << "\n";
    return ss.str();
}

bool compareByDist(const NodeIntersection& a, const NodeIntersection& b)
{
    return a.dist_ < b.dist_;
}

bool Mesh::intersect(const LightRay& lr, Vector3& point, Vector3& normal, double& dist) const
{
    LightRay lrInFrame;
    lrInFrame.dir_ = f_.vecFromWorld(lr.dir_);
    lrInFrame.origin_ = f_.pointFromWorld(lr.origin_);
    std::vector<NodeIntersection> nodeIntersections;
    if(not hbv_.intersect(lrInFrame, point, normal, dist, nodeIntersections))
    {
        return false;
    }

    std::sort(nodeIntersections.begin(), nodeIntersections.end(), compareByDist);

    if(simplifiedRender_)
    {
        point = f_.pointToWorld(nodeIntersections[0].point_);
        normal = f_.vecToWorld(nodeIntersections[0].normal_);
        dist = nodeIntersections[0].dist_;
        return true;
    }

    dist = INFINITY_d();
    size_t minIndex = -1;
    bool impact = false;
    for (size_t nIndex = 0; nIndex < nodeIntersections.size(); nIndex++)
    {
        bool impactNode = false;
        for (size_t j = 0; j < nodeIntersections[nIndex].node_->includedIndices_.size(); j++)
        {
            int tIndex = nodeIntersections[nIndex].node_->includedIndices_[j];
            Vector3 triPoint;
            Vector3 triNormal;
            double triDist;
            bool triImpact;
            triImpact = polygons_[tIndex].intersect(lrInFrame, triPoint, triNormal, triDist);
            if(triImpact and triDist < dist)
            {
                impactNode = true;
                impact = true;
                dist = triDist;
                point = triPoint;
                normal = triNormal;
                minIndex = tIndex;
            }
        }
        //TODO: The following optimization creates aberations, should fix it. Commented for now.
        //if(impactNode)
        //    break;
    }
    point = f_.pointToWorld(point);
    normal = f_.vecToWorld(normal);
    return impact;
}

bool Mesh::isInHalfSpace(const Vector3& point, const Vector3& normal, const Vector3& specDir, double& cosAngleDiffuse, double& cosAnglePhong) const
{
    return false;
}

std::string MeshData::describe() const
{
    std::stringstream ss;
    ss << "=== MeshData ===\n";
    ss << GeometryData::describe();
    ss << "path: " << path << "\n";
    ss << "depthHBV: " << depthHBV << "\n";
    return ss.str();
}

std::string Mesh::to_string(const MeshType& mt) const
{
    if(mt == MeshType::stl)
        return std::string("stl");
    else if(mt == MeshType::obj)
        return std::string("obj");
    else
        return std::string("unknown");
}
