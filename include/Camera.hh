#pragma once
#include <iostream>
#include "Vector3.hh"

class Camera
{
public:
  Camera (const Vector3& pos, const Vector3& dir);
  virtual ~Camera ();

private:
  Vector3 pos_;
  Vector3 dir_;
};
