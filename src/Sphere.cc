#include "Sphere.hh"

Sphere::Sphere(const Vector3& center, double radius, const Vector3RGB& color)
  : center_(center),
    radius_(radius),
    color_(color)
{
}

Sphere::~Sphere()
{
}

std::string Sphere::describe() const
{
  std::stringstream ss;
  ss << "=== Sphere ===\n";
  ss << "center: " << center_ << "\n";
  ss << "radius: " << radius_ << "\n";
  ss << "color: " << color_ << "\n";
  return ss.str();
}

Pixel Sphere::intersect(const LightRay& lr, bool& contact, double& dist) const
{
  // Compute dist between light ray and sphere center
  double t = (lr.dir_.dot(lr.origin_ - center_))/lr.dir_.norm();
  Vector3 closestPoint = lr.origin_ - (lr.dir_ * t);
  dist = (closestPoint - center_).norm();
  if(dist < radius_)
  {
    contact = true;
    return Pixel(lr.px_, lr.py_, 1, color_.r_, color_.g_, color_.b_);
  }
  else
  {
    contact = false;
    return Pixel(lr.px_, lr.py_, 1, 0,0,0);
  }
}

std::ostream& operator<<(std::ostream& os, const Sphere& s)
{
  os << s.describe();
}
