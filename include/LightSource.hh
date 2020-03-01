#pragma once
#include <iostream>
#include <sstream>
#include <string>

#include "Vector3.hh"

class LightSource
{
public:
  LightSource (const Vector3& pos, double ratio, double radius, const Vector3RGB& rgb);
  virtual ~LightSource ();
  std::string describe() const;

  Vector3 pos_;
  double ratio_;
  double radius_;
  Vector3RGB rgb_;

private:
};
