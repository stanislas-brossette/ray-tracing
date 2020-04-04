#pragma once
#include <vector>

#include "stl_reader/stl_reader.h"

#include "PerformanceTracker.hh"
#include "dataStructures.hh"
#include "Frame3.hh"
#include "geometries/Geometry.hh"
#include "geometries/Polygon.hh"
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
    void initTriangles();
    //The planes normal is f_.z_
    bool intersect(const LightRay& incident, Vector3& point, Vector3& normal, double& dist) const;
    bool isInHalfSpace(const Vector3& point, const Vector3& normal, const Vector3& specDir, double& cosAngleDiffuse, double& cosAnglePhong) const;

    std::vector<Polygon> triangles_;
    std::string path_;

};
