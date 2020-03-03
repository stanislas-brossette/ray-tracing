#pragma once

#include <iostream>
#include "Frame3.hh"

class Geometry
{
public:
  Geometry ();
  virtual ~Geometry ();
  virtual std::string describe() const = 0;
  virtual bool intersect(const LightRay& incident, Vector3& point, Vector3& normal) const = 0;

  Frame3 f_;
};
