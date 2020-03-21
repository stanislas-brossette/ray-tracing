#pragma once

#include "dataStructures.hh"
#include "Frame3.hh"
#include "Geometry.hh"

class Polygon : public Geometry
{
public:
    Polygon ();
    Polygon (PolygonData* pData);
    Polygon(const Frame3& f);
    virtual ~Polygon ();
    std::string describe() const;
    //The planes normal is f_.z_
    bool intersect(const LightRay& incident, Vector3& point, Vector3& normal, double& dist) const;
    bool isInHalfSpace(const Vector3& point, const Vector3& normal, double& cosAngle) const;
};

