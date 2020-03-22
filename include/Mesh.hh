#pragma once
#include <vector>

#include "stl_reader/stl_reader.h"

#include "dataStructures.hh"
#include "Frame3.hh"
#include "Geometry.hh"
#include "Vector3.hh"
#include "Polygon.hh"
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
    bool isInHalfSpace(const Vector3& point, const Vector3& normal, double& cosAngle) const;

    std::vector<Polygon> triangles_;
    std::string path_;

};
