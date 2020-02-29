#include "AmbiantLight.hh"

AmbiantLight::AmbiantLight (double r, const Vector3RGB& v)
  : ratio_(r),
    rgb_(v)
{
}

AmbiantLight::~AmbiantLight()
{
}

std::string AmbiantLight::describe() const
{
  std::stringstream ss;
  ss << "=== AmbiantLight ===\n";
  ss << "ratio: " << ratio_ << "\n";
  ss << "rgb: " << rgb_;
  return ss.str();
}

