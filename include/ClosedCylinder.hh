#pragma once

#include "Frame3.hh"
#include "Geometry.hh"
#include "Cylinder.hh"
#include "Plane.hh"
#include "utils.hh"

class ClosedCylinder : public Geometry
{
public:
    ClosedCylinder ();
    ClosedCylinder(const Frame3& f, double radius, double length = 100000000);
    virtual ~ClosedCylinder ();
    std::string describe() const;
    //The Cylinders normal is f_.z_
    bool intersect(const LightRay& incident, Vector3& point, Vector3& normal, double& dist) const;
    bool isInHalfSpace(const Vector3& point, const Vector3& normal, double& cosAngle) const;

    double radius_;
    double length_;

    Cylinder cylinder_;
    Plane topPlane_;
    Plane bottomPlane_;
};


