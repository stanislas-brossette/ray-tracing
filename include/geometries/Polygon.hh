#pragma once
#include <vector>

#include "PerformanceTracker.hh"
#include "dataStructures.hh"
#include "Frame3.hh"
#include "geometries/Geometry.hh"
#include "Vector3.hh"

class Polygon : public Geometry
{
public:
    Polygon ();
    Polygon (PolygonData* pData);
    Polygon(const Frame3& f, const std::vector<Vector2>& p);
    Polygon(const Frame3& f, const std::vector<Vector2>& p, const std::vector<Vector3>& n);
    virtual ~Polygon ();
    std::string describe() const;
    //The planes normal is f_.z_
    bool intersect(const LightRay& incident, Vector3& point, Vector3& normal, double& dist) const;
    bool isInHalfSpace(const Vector3& point, const Vector3& normal, const Vector3& specDir, double& cosAngleDiffuse, double& cosAnglePhong) const;

    std::vector<Vector2> points_;
    std::vector<Vector3> normals_;
    double a0, a1, b0, b1, c0, c1;
};

