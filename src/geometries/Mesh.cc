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
    else if(extension.compare("STL") == 0 or extension.compare("stl") == 0)
        meshType_ = MeshType::stl;
    else
        std::cout << "ERROR, mesh type unknown" << std::endl;
}

void Mesh::loadSTLMesh()
{
    std::vector<float> coords, normals;
    std::vector<unsigned int> tris, solids;
    stl_reader::ReadStlFile (path_.c_str(), coords, normals, tris, solids);

    int numTris = tris.size() / 3;

    for(int itri = 0; itri < numTris; ++itri)
    {
        // load vertices in vertices_
        float* c = &coords[3 * tris [3 * itri]];
        vertices_.push_back({c[0], c[1], c[2]});
        c = &coords[3 * tris [3 * itri + 1]];
        vertices_.push_back({c[0], c[1], c[2]});
        c = &coords[3 * tris [3 * itri + 2]];
        vertices_.push_back({c[0], c[1], c[2]});

        // Extend HBV0
        const Vector3& P0(vertices_[3*itri]);
        const Vector3& P1(vertices_[3 * itri + 1]);
        const Vector3& P2(vertices_[3 * itri + 2]);
        hbv_.extendByTriangle(P0, P1, P2, itri);

        // load normal in normals_
        float* n = &normals [3 * itri];
        vertexNormals_.push_back({n[0], n[1], n[2]});
        vertexNormals_[itri].normalize();

        // load texture in textures_
        vertexTextures_.push_back({0,0});

        // Compute polygon
        const Vector3& vz(vertexNormals_[itri]);
        Vector3 vx = (P1 - P0).normalize();
        Vector3 vy = vz.cross(vx);
        Frame3 tFrame(P0, vx, vy, vz);
        std::vector<Vector2> points(3); // only triangles in .stl
        points[0] = Vector2(0,0);
        points[1] = Vector2((P2 - P0).dot(vx), (P2 - P0).dot(vy));
        points[2] = Vector2((P1 - P0).dot(vx), (P1 - P0).dot(vy));
        polygons_.push_back({tFrame, points});

        // Fill faces_
        faces_.push_back({{3*itri, itri, itri},{3*itri+1, itri, itri},{3*itri+2, itri, itri}});
    }
}

void Mesh::loadOBJMesh()
{
    // Read obj file
    std::ifstream objFile(path_);
    std::string line;
    while (std::getline(objFile, line))
    {
        std::vector<std::string> tokens = split(line, " ");
        if(tokens[0].compare("v") == 0)
            vertices_.push_back({std::stod(tokens[1]), -std::stod(tokens[3]), std::stod(tokens[2])});
        else if(tokens[0].compare("vn") == 0)
            vertexNormals_.push_back({std::stod(tokens[1]), -std::stod(tokens[3]), std::stod(tokens[2])});
        else if(tokens[0].compare("vt") == 0)
            vertexTextures_.push_back({std::stod(tokens[1]), std::stod(tokens[2])});
        else if(tokens[0].compare("f") == 0)
        {
            faces_.push_back({});
            for (size_t iVertex = 1; iVertex < tokens.size(); iVertex++)
            {
                std::vector<std::string> indices = split(tokens[iVertex], "/");
                int vertex = std::stoi(indices[0]) - 1;
                int texture = -1;
                if(indices[1].size() > 0)
                    texture = std::stoi(indices[1]) - 1;
                int normal = std::stoi(indices[2]) - 1;
                faces_.back().push_back({vertex, texture, normal});
            }
        }
    }

    // Extend HBV0 and compute 2D polygons
    for (size_t iFace = 0; iFace < faces_.size(); iFace++)
    {
        const std::vector<FaceVertex>& face(faces_[iFace]);
        std::vector<Vector3> points;
        std::vector<Vector3> normals;
        for (size_t iPoints = 0; iPoints < face.size(); iPoints++)
        {
            points.push_back(vertices_[face[iPoints].vertex]);
            normals.push_back(vertexNormals_[face[iPoints].normal]);
        }
        hbv_.extendByPolygon(points);

        Vector3 vz = ((points[2] - points[0]).cross(points[1] - points[0])).normalize();
        Vector3 vx = (points[1] - points[0]).normalize();
        Vector3 vy = vz.cross(vx);
        Frame3 tFrame(points[0], vx, vy, vz);
        std::vector<Vector2> points2D;
        points2D.push_back({0,0});
        for (size_t iPoints = 1; iPoints < points.size(); iPoints++)
        {
            points2D.push_back({(points[iPoints]-points[0]).dot(vx),
                    (points[iPoints]-points[0]).dot(vy)});
        }
        polygons_.push_back({tFrame, points2D, normals});
    }
}

void Mesh::initTriangles()
{
    switch(meshType_)
    {
        case MeshType::stl:
            loadSTLMesh();
            break;
        case MeshType::obj:
            loadOBJMesh();
            break;
        default:
            std::cout << "ERROR, mesh type unknown" << std::endl;
            break;
    }
    hbv_.finishFirstPass();
    for(size_t iFace = 0; iFace < faces_.size(); ++iFace)
    {
        std::vector<Vector3> points;
        const std::vector<FaceVertex>& face(faces_[iFace]);
        for (size_t iPoints = 0; iPoints < face.size(); iPoints++)
        {
            points.push_back(vertices_[face[iPoints].vertex]);
        }
        hbv_.populateWithPolygon(points, iFace);
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

bool Mesh::intersect(const LightRay& lr, Vector3& point, Vector3& normal, double& dist, bool verbose) const
{
    if(verbose)
        std::cout << "Mesh::intersect" << std::endl;
    LightRay lrInFrame;
    lrInFrame.dir_ = f_.vecFromWorld(lr.dir_);
    lrInFrame.origin_ = f_.pointFromWorld(lr.origin_);
    std::vector<NodeIntersection> nodeIntersections;
    if(not hbv_.intersect(lrInFrame, point, normal, dist, nodeIntersections, verbose))
    {
        return false;
    }
    if(verbose)
        std::cout << "Intersection with hbv" << std::endl;

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
            //Polygon::intersect receives lr in meshes frame and computes triPoint and triNormal in meshes frame
            triImpact = polygons_[tIndex].intersect(lrInFrame, triPoint, triNormal, triDist, verbose);
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
