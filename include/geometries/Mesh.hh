#pragma once
#include <vector>
#include <fstream>

#include "mesh_reader/stl_reader.h"

#include "PerformanceTracker.hh"
#include "dataStructures.hh"
#include "Frame3.hh"
#include "geometries/Geometry.hh"
#include "geometries/Polygon.hh"
#include "geometries/BoundingPolyhedron.hh"
#include "geometries/HierarchyBoundingVolume.hh"
#include "Vector3.hh"
#include "utils.hh"

class Mesh : public Geometry
{
public:
    Mesh ();
    Mesh (MeshData* mData);
    Mesh (const Frame3& f, std::string path);
    virtual ~Mesh ();
    std::string describe() const;
    void readPath();
    void initTriangles();
    void loadSTLMesh();
    void loadOBJMesh();
    //The planes normal is f_.z_
    bool intersect(const LightRay& incident, Vector3& point, Vector3& normal, double& dist) const;
    bool isInHalfSpace(const Vector3& point, const Vector3& normal, const Vector3& specDir, double& cosAngleDiffuse, double& cosAnglePhong) const;

    struct FaceVertex
    {
        int vertex;
        int texture;
        int normal;
    };

    enum MeshType
    {
        stl,
        obj
    };
    MeshType meshType_;
    std::string to_string(const MeshType& mt) const;

    std::string path_;

    std::vector<Polygon> polygons_;
    std::vector<Vector3> vertices_;
    std::vector<Vector3> vertexNormals_;
    std::vector<Vector2> vertexTextures_;
    std::vector<std::vector<FaceVertex> > faces_;

    HierarchyBoundingVolume hbv_;
};
