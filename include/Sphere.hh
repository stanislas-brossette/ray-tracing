#pragma once
#include <iostream>
#include <sstream>
#include <string>

#include "Item.hh"
#include "Vector3.hh"
#include "Pixel.hh"

class Sphere : public Item
{
public:
  Sphere(const Vector3& center, double radius, const Vector3RGB& color);
  virtual ~Sphere ();
  std::string describe() const;
  Pixel intersect(const LightRay& lr, LightRay& lr2, bool& contact, double& distance) const;

  friend std::ostream& operator<<(std::ostream& os, const Sphere& s);

  Vector3 center_;
  double radius_;
  Vector3RGB color_;
};

