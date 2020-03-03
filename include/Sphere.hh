#pragma once

#include "Frame3.hh"
#include "Geometry.hh"

class Sphere : public Geometry
{
public:
  Sphere ();
  virtual ~Sphere ();
  std::string describe() const;
  bool intersect(const LightRay& incident, Vector3& point, Vector3& normal) const;

  double radius_;
};
