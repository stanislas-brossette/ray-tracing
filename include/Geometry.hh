#pragma once

#include <iostream>
#include "Frame3.hh"
#include "Vector3.hh"
#include "LightRay.hh"

class Geometry
{
public:
  Geometry ();
  Geometry (const Frame3& f);
  virtual ~Geometry ();
  virtual std::string describe() const = 0;
  virtual bool intersect(const LightRay& incident, Vector3& point, Vector3& normal, double& dist) const = 0;

  Frame3 f_;
};
