#pragma once
#include <iostream>
#include <sstream>
#include <string>
#include <cstdlib>

#include "Vector3.hh"
#include "Frame3.hh"
#include "LightRay.hh"

class Camera
{
public:
  Camera (const Frame3& frame, int fovX, int fovY, int resX, int resY);
  virtual ~Camera ();
  std::string describe() const;
  LightRay castRandomRay() const;

  Frame3 frame_;
  int fovX_;
  int fovY_;
  int resX_;
  int resY_;
};
