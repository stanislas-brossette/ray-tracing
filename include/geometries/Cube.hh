#pragma once

#include "dataStructures.hh"
#include "Frame3.hh"
#include "geometries/Geometry.hh"
#include "geometries/Polygon.hh"
#include "utils.hh"

class Cube : public Geometry
{
public:
    Cube ();
    Cube (CubeData* cData);
    Cube(const Frame3& f, double size);
    virtual ~Cube ();
    std::string describe() const;
    //The Cylinders normal is f_.z_
    bool intersect(const LightRay& incident, Vector3& point, Vector3& normal, double& dist, bool verbose = false) const;
    bool isInHalfSpace(const Vector3& point, const Vector3& normal, const Vector3& specDir, double& cosAngleDiffuse, double& cosAnglePhong) const;
    void initPlanes();

    double size_;

    Polygon topPlane_;
    Polygon bottomPlane_;
    Polygon frontPlane_;
    Polygon backPlane_;
    Polygon rightPlane_;
    Polygon leftPlane_;
};



