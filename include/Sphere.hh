#pragma once
#include <iostream>
#include <sstream>
#include <string>

#include "Item.hh"
#include "Vector3.hh"

class Sphere : public Item
{
public:
  Sphere (const Vector3& c, double r);
  virtual ~Sphere ();
  std::string describe() const;

  friend std::ostream& operator<<(std::ostream& os, const Sphere& s);

  Vector3 center_;
  double radius_;
};

