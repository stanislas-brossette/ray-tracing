#pragma once

#include "Frame3.hh"
#include "Geometry.hh"

class Sphere : public Geometry
{
public:
  Sphere ();
  Sphere(const Frame3& f, double radius);
  virtual ~Sphere ();
  std::string describe() const;
  bool intersect(const LightRay& incident, Vector3& point, Vector3& normal, double& dist) const;

  double radius_;
};
