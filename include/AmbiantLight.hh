#pragma once
#include <iostream>
#include <sstream>
#include <string>

#include "Vector3.hh"

class AmbiantLight
{
public:
  AmbiantLight ();
  AmbiantLight (double alpha, const Vector3RGB& v);
  virtual ~AmbiantLight ();
  std::string describe() const;
  
  double alpha_;
  Vector3RGB rgb_;
};
