#pragma once
#include <iostream>
#include <sstream>
#include <string>
#include "Vector3.hh"

class LightRay
{
public:
  LightRay (Vector3 origin, Vector3 dir, double px, double py);
  virtual ~LightRay ();
  std::string describe() const;

  Vector3 origin_;
  Vector3 dir_;
  double px_;
  double py_;
};
