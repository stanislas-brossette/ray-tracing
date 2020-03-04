#pragma once
#include <iostream>
#include <sstream>
#include <string>
#include "Vector3.hh"

class LightRay
{
public:
  LightRay ();
  LightRay (Vector3 origin, Vector3 dir);
  virtual ~LightRay ();
  std::string describe() const;

  Vector3 origin_;
  Vector3 dir_;
};
