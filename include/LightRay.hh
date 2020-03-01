#pragma once
#include <iostream>
#include "Vector3.hh"

class LightRay
{
public:
  LightRay (Vector3 origin, Vector3 dir);
  virtual ~LightRay ();

  Vector3 origin_;
  Vector3 dir_;
};
