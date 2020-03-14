#pragma once
#include <iostream>
#include <sstream>
#include "Geometry.hh"
#include "Material.hh"

struct itemData
{
    std::string name;
    materialData* mData;
    geometryData* gData;
    std::string describe() const;
};

class Item
{
public:
    Item (Geometry* g, Material* m);
    virtual ~Item ();

    std::string describe() const;
    bool intersect(const LightRay& incident, Vector3& point, Vector3& normal, double& dist) const;
    bool isInHalfSpace(const Vector3& point, const Vector3& normal, double& cosAngle) const;

    Geometry* geometry_;
    Material* material_;
};
