#include "Mesh.hh"

Mesh::Mesh()
    : Geometry(),
    triangles_(),
    path_()
{
}

Mesh::Mesh (const Frame3& f, std::string path)
    : Geometry(f),
    triangles_(),
    path_(path)
{
    if(not fileExists(path_))
        path_ = std::string(MESHES) + path_;
    if(not fileExists(path_))
        std::cout << "ERROR, mesh file does not exist: " << path << std::endl;
    initTriangles();
}

Mesh::Mesh (MeshData* mData)
    : Geometry(mData),
    triangles_(),
    path_(mData->path)
{
    if(not fileExists(path_))
        path_ = std::string(MESHES) + path_;
    if(not fileExists(path_))
        std::cout << "ERROR, mesh file does not exist" << std::endl;
    initTriangles();
}

Mesh::~Mesh ()
{
}

void Mesh::initTriangles()
{
    std::vector<float> coords, normals;
    std::vector<unsigned int> tris, solids;
    stl_reader::ReadStlFile (path_.c_str(), coords, normals, tris, solids);

    Vector3 bary;
    bary.x_ = 0;
    bary.y_ = 0;
    bary.z_ = 0;
    for (size_t i = 0; i < coords.size()/3; i++)
    {
        bary.x_ += coords[3*i];
        bary.y_ += coords[3*i + 1];
        bary.z_ += coords[3*i + 2];
    }
    bary = bary * (3.0/coords.size());
    std::cout << "bary: " << bary << std::endl;
    double radiusBoundingSphere = -1;
    for (size_t i = 0; i < coords.size()/3; i++)
    {
        Vector3 p(coords[3*i], coords[3*i + 1], coords[3*i + 2]);
        double bd = (p-bary).squaredNorm();
        if(bd > radiusBoundingSphere)
            radiusBoundingSphere = bd;
    }
    radiusBoundingSphere = std::sqrt(radiusBoundingSphere);
    std::cout << "radiusBoundingSphere: " << radiusBoundingSphere << std::endl;

    Frame3 fBV(f_.o_ + bary, f_.vx_, f_.vy_, f_.vz_);
    bv_ = BoundingVolume(fBV, radiusBoundingSphere);

    size_t numTris = tris.size() / 3;
    triangles_.resize(numTris);
    for(size_t itri = 0; itri < numTris; ++itri)
    {
        //std::cout << "coordinates of triangle " << itri << ": ";
        //for(size_t icorner = 0; icorner < 3; ++icorner)
        //{
        //    float* c = &coords[3 * tris [3 * itri + icorner]];
        //    std::cout << "(" << c[0] << ", " << c[1] << ", " << c[2] << ") ";
        //}
        //std::cout << std::endl;

        float* n = &normals [3 * itri];
        //std::cout << "normal of triangle " << itri << ": " << "(" << n[0] << ", " << n[1] << ", " << n[2] << ")\n";

        float* c = &coords[3 * tris [3 * itri]];
        Vector3 P0(c[0], c[1], c[2]);
        c = &coords[3 * tris [3 * itri + 1]];
        Vector3 P1(c[0], c[1], c[2]);
        c = &coords[3 * tris [3 * itri + 2]];
        Vector3 P2(c[0], c[1], c[2]);

        //double radius = (P0 - f_.o_).squaredNorm();
        //if(radius > radiusBoundingSphere)
        //    radiusBoundingSphere = radius;
        //radius = (P1 - f_.o_).squaredNorm();
        //if(radius > radiusBoundingSphere)
        //    radiusBoundingSphere = radius;
        //radius = (P2 - f_.o_).squaredNorm();
        //if(radius > radiusBoundingSphere)
        //    radiusBoundingSphere = radius;

        Vector3 vz(n[0], n[1], n[2]);
        vz.normalize();
        Vector3 vx = (P1 - P0).normalize();
        Vector3 vy = vz.vectorial(vx);
        Frame3 tFrame(f_.o_ + P0, vx, vy, vz);
        std::vector<Vector2> points(3);
        points[0] = Vector2(0,0);
        points[1] = Vector2((P2 - P0).dot(vx), (P2 - P0).dot(vy));
        points[2] = Vector2((P1 - P0).dot(vx), (P1 - P0).dot(vy));
        triangles_[itri] = Polygon(tFrame, points);
        //std::cout << triangles_[itri].describe() << std::endl;
    }
    std::cout << "triangles_.size(): " << triangles_.size() << std::endl;
}

std::string Mesh::describe() const
{
    std::stringstream ss;
    ss << "=== Mesh ===\n";
    ss << "origin: " << f_.o_ << "\n";
    ss << "nTri: " << triangles_.size() << "\n";
    return ss.str();
}

bool Mesh::intersect(const LightRay& lr, Vector3& point, Vector3& normal, double& dist) const
{
    if(not bv_.intersect(lr))
        return false;

    dist = INFINITY_d();
    size_t minIndex = -1;
    bool impact = false;
    //std::cout << "triangles_.size(): " << triangles_.size() << std::endl;
    for (size_t i = 0; i < triangles_.size(); i++)
    {
        Vector3 triPoint;
        Vector3 triNormal;
        double triDist;
        bool triImpact;
        //std::cout << "triangles_[i].describe(): " << triangles_[i].describe() << std::endl;
        triImpact = triangles_[i].intersect(lr, triPoint, triNormal, triDist);
        if(triImpact and triDist < dist)
        {
            impact = true;
            dist = triDist;
            point = triPoint;
            normal = triNormal;
            minIndex = i;
        }
    }
    //if(impact)
    //{
    //    std::cout << "\nImpact with mesh" << std::endl;
    //    std::cout << "dist: " << dist << std::endl;
    //    std::cout << "point: " << point << std::endl;
    //    std::cout << "normal: " << normal << std::endl;
    //    std::cout << "minIndex: " << minIndex << std::endl;
    //}
    return impact;
}

bool Mesh::isInHalfSpace(const Vector3& point, const Vector3& normal, double& cosAngle) const
{
    return false;
}

std::string MeshData::describe() const
{
    std::stringstream ss;
    ss << "=== MeshData ===\n";
    ss << GeometryData::describe();
    return ss.str();
}
