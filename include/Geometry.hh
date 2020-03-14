#pragma once

#include <iostream>
#include "Frame3.hh"
#include "Vector3.hh"
#include "LightRay.hh"

struct GeometryData
{
    std::string type = "";
    Vector3 pos = Vector3(0,0,0);
    Vector3 rotAxis = Vector3(1,0,0);
    double rotAngle = 0;
    virtual std::string describe() const;
};

class Geometry
{
public:
    Geometry ();
    Geometry (const Frame3& f);
    Geometry (GeometryData* gData);
    virtual ~Geometry ();
    virtual std::string describe() const = 0;
    virtual bool intersect(const LightRay& incident, Vector3& point, Vector3& normal, double& dist) const = 0;
    virtual bool isInHalfSpace(const Vector3& point, const Vector3& normal, double& cosAngle) const = 0;

    Frame3 f_;
};
