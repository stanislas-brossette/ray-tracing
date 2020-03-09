#pragma once

#include "Frame3.hh"
#include "Geometry.hh"

class Cylinder : public Geometry
{
public:
    Cylinder ();
    Cylinder(const Frame3& f, double radius);
    virtual ~Cylinder ();
    std::string describe() const;
    //The Cylinders normal is f_.z_
    bool intersect(const LightRay& incident, Vector3& point, Vector3& normal, double& dist) const;
    bool isInHalfSpace(const Vector3& point, const Vector3& normal, double& cosAngle) const;

    double radius_;
};

