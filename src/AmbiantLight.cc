#include "AmbiantLight.hh"

AmbiantLight::AmbiantLight()
  : alpha_(0),
    rgb_()
{
}

AmbiantLight::AmbiantLight (double a, const Vector3RGB& v)
  : alpha_(a),
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
    ss << "alpha: " << alpha_ << "\n";
    ss << "rgb: " << rgb_;
    return ss.str();
}

