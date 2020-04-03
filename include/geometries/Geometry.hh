#pragma once

#include <iostream>

#include "dataStructures.hh"
#include "BoundingVolume.hh"
#include "Frame3.hh"
#include "Vector3.hh"
#include "LightRay.hh"

class Geometry
{
public:
    Geometry ();
    Geometry (const Frame3& f);
    Geometry (GeometryData* gData);
    virtual ~Geometry ();
    virtual std::string describe() const = 0;
    virtual bool intersect(const LightRay& incident, Vector3& point, Vector3& normal, double& dist) const = 0;
    virtual bool isInHalfSpace(const Vector3& point, const Vector3& normal, const Vector3& specDir, double& cosAngleDiffuse, double& cosAnglePhong) const = 0;

    Frame3 f_;
    BoundingVolume bv_;
};
