#pragma once

#include "dataStructures.hh"
#include "Frame3.hh"
#include "geometries/Geometry.hh"

class Plane : public Geometry
{
public:
    Plane ();
    Plane (PlaneData* pData);
    Plane(const Frame3& f);
    virtual ~Plane ();
    std::string describe() const;
    //The planes normal is f_.z_
    bool intersect(const LightRay& incident, Vector3& point, Vector3& normal, double& dist) const;
    bool isInHalfSpace(const Vector3& point, const Vector3& normal, const Vector3& specDir, double& cosAngleDiffuse, double& cosAnglePhong) const;
};
