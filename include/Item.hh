#pragma once
#include <iostream>
#include <sstream>
#include "Geometry.hh"
#include "Material.hh"

class Item
{
public:
  Item (Geometry* g, Material* m);
  virtual ~Item ();

  std::string describe() const;
  bool intersect(const LightRay& incident, Vector3& point, Vector3& normal, double& dist) const;

  Geometry* geometry_;
  Material* material_;
};
