#pragma once
#include <iostream>
#include <sstream>
#include <string>

#include "Vector3.hh"
#include "Pixel.hh"
#include "LightRay.hh"

class LightSource
{
public:
  LightSource (const Vector3& center, double ratio, double radius, const Vector3RGB& rgb);
  virtual ~LightSource ();
  std::string describe() const;
  Pixel intersect(const LightRay& lr, bool& contact, double& distance) const;

  Vector3 center_;
  double ratio_;
  double radius_;
  Vector3RGB rgb_;

private:
};
