#pragma once
#include <iostream>
#include <sstream>
#include "Geometry.hh"
#include "Sphere.hh"
#include "Cylinder.hh"
#include "ClosedCylinder.hh"
#include "Plane.hh"
#include "Material.hh"

struct ItemData
{
    std::string name;
    MaterialData* mData;
    GeometryData* gData;
    std::string describe() const;
};

class Item
{
public:
    Item (Geometry* g, Material* m);
    Item (const ItemData& iData);
    virtual ~Item ();

    std::string describe() const;
    bool intersect(const LightRay& incident, Vector3& point, Vector3& normal, double& dist) const;
    bool isInHalfSpace(const Vector3& point, const Vector3& normal, double& cosAngle) const;

    Geometry* geometry_;
    Material* material_;
    std::string name_;
};
