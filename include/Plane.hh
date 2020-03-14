#pragma once

#include "Frame3.hh"
#include "Geometry.hh"

struct planeData : public geometryData
{
    std::string describe() const;
};

class Plane : public Geometry
{
public:
    Plane ();
    Plane(const Frame3& f);
    virtual ~Plane ();
    std::string describe() const;
    //The planes normal is f_.z_
    bool intersect(const LightRay& incident, Vector3& point, Vector3& normal, double& dist) const;
    bool isInHalfSpace(const Vector3& point, const Vector3& normal, double& cosAngle) const;
};
