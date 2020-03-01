#pragma once
#include <iostream>
#include <sstream>
#include <string>
#include <cstdlib>

#include "Vector3.hh"
#include "LightRay.hh"

class Camera
{
public:
  Camera (const Vector3& pos, const Vector3& dir, int fovX, int fovY, int resX, int resY);
  virtual ~Camera ();
  std::string describe() const;
  LightRay castRandomRay() const;

  Vector3 pos_;
  Vector3 dir_;
  int fovX_;
  int fovY_;
  int resX_;
  int resY_;
};
