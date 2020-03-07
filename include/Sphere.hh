#pragma once

#include "Frame3.hh"
#include "Geometry.hh"

class Sphere : public Geometry
{
public:
    Sphere ();
    Sphere(const Frame3& f, double radius);
    virtual ~Sphere ();
    std::string describe() const;
    bool intersect(const LightRay& incident, Vector3& point, Vector3& normal, double& dist) const;
    bool isInHalfSpace(const Vector3& point, const Vector3& normal, double& cosAngle) const;

    double radius_;
};
