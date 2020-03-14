#include "AmbiantLight.hh"

AmbiantLight::AmbiantLight()
  : alpha_(0),
    rgb_()
{
}

AmbiantLight::AmbiantLight(const AmbiantData& aData)
  : alpha_(aData.intensity),
    rgb_(aData.color)
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
    ss << "rgb: " << rgb_ << "\n";
    return ss.str();
}

std::string AmbiantData::describe() const
{
    std::stringstream ss;
    ss << "=== AmbiantData ===\n";
    ss << "intensity: " << intensity << "\n";
    ss << "color: " << color << "\n";
    return ss.str();
}
