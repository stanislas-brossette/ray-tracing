#include "LightSource.hh"

LightSource::LightSource(const Vector3& center, double ratio, double radius, const Vector3RGB& rgb)
  : center_(center),
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
  ss << "center: " << center_ << "\n";
  ss << "ratio: " << ratio_ << "\n";
  ss << "radius: " << radius_ << "\n";
  ss << "rgb: " << rgb_;
  return ss.str();
}

Pixel LightSource::intersect(const LightRay& lr, bool& contact, double& distance) const
{
  // Compute dist between light ray and sphere center
  double t = (lr.dir_.dot(lr.origin_ - center_))/lr.dir_.norm();
  Vector3 closestPoint = lr.origin_ - (lr.dir_ * t);
  double orthogDist = (closestPoint - center_).norm();
  if(orthogDist < radius_ and orthogDist > 0)
  {
    contact = true;
    return Pixel(lr.px_, lr.py_, 1, rgb_.r_, rgb_.g_, rgb_.b_);
  }
  else
  {
    contact = false;
    return Pixel(lr.px_, lr.py_, 1, 0,0,0);
  }
}
