#pragma once
#include <iostream>
#include <sstream>

#include "dataStructures.hh"
#include "PerformanceTracker.hh"
#include "geometries/Geometry.hh"
#include "geometries/Sphere.hh"
#include "geometries/Cylinder.hh"
#include "geometries/ClosedCylinder.hh"
#include "geometries/Plane.hh"
#include "geometries/Cube.hh"
#include "geometries/Mesh.hh"
#include "geometries/Polygon.hh"
#include "Material.hh"

class Item
{
public:
    Item ();
    Item (Geometry* g, Material* m);
    Item (const ItemData& iData);
    virtual ~Item ();

    void load(const ItemData& iData);
    std::string describe() const;
    bool intersect(const LightRay& incident, Vector3& point, Vector3& normal, double& dist, bool verbose = false) const;
    bool isInHalfSpace(const Vector3& point, const Vector3& normal, const Vector3& specDir, double& cosAngleDiffuse, double& cosAnglePhong) const;

    Geometry* geometry_;
    Material* material_;
    std::string name_;
};
