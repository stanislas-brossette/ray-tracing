#include "AmbiantLight.hh"

AmbiantLight::AmbiantLight()
  : intensity_(0),
    color_()
{
}

AmbiantLight::AmbiantLight(const AmbiantData& aData)
  : intensity_(aData.intensity),
    color_(aData.color)
{
}

AmbiantLight::AmbiantLight (double a, const Vector3RGB& v)
  : intensity_(a),
    color_(v)
{
}

AmbiantLight::~AmbiantLight()
{
}

std::string AmbiantLight::describe() const
{
    std::stringstream ss;
    ss << "=== AmbiantLight ===\n";
    ss << "intensity: " << intensity_ << "\n";
    ss << "color: " << color_ << "\n";
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

std::ostream& operator<<(std::ostream& os, const AmbiantLight& a)
{
    os << a.describe();
    return os;
}
