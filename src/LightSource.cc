#include "LightSource.hh"

LightSource::LightSource(const Vector3& pos, double ratio, double radius, const Vector3RGB& rgb)
  : pos_(pos),
    ratio_(ratio),
    radius_(radius),
    rgb_(rgb)
{
}

LightSource::~LightSource()
{
}

std::string LightSource::describe() const
{
  std::stringstream ss;
  ss << "=== Light Source ===\n";
  ss << "pos: " << pos_ << "\n";
  ss << "ratio: " << ratio_ << "\n";
  ss << "radius: " << radius_ << "\n";
  ss << "rgb: " << rgb_;
  return ss.str();
}

