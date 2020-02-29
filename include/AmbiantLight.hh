#pragma once
#include <iostream>
#include <sstream>
#include <string>

#include "Vector3.hh"

class AmbiantLight
{
public:
  AmbiantLight (double r, const Vector3RGB& v);
  virtual ~AmbiantLight ();
  std::string describe() const;
  
  double ratio_;
  Vector3RGB rgb_;
};
