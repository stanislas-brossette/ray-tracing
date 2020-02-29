#pragma once
#include <iostream>
#include <sstream>
#include <string>

#include "Vector3.hh"

class Camera
{
public:
  Camera (const Vector3& pos, const Vector3& dir, double fov);
  virtual ~Camera ();
  std::string describe() const;

  Vector3 pos_;
  Vector3 dir_;
  double fov_;
};
