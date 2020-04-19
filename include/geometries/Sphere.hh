#pragma once

#include "dataStructures.hh"
#include "Frame3.hh"
#include "geometries/Geometry.hh"
#include "utils.hh"

class Sphere : public Geometry
{
public:
    Sphere ();
    Sphere (SphereData* sData);
    Sphere(const Frame3& f, double radius);
    virtual ~Sphere ();
    std::string describe() const;
    bool intersect(const LightRay& incident, Vector3& point, Vector3& normal, double& dist, bool verbose = false) const;
    bool isInHalfSpace(const Vector3& point, const Vector3& normal, const Vector3& specDir, double& cosAngleDiffuse, double& cosAnglePhong) const;

    double radius_;
};
