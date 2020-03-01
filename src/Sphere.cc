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

bool Sphere::intersect(const LightRay& lr) const
{
  std::cout << "Sphere::intersect" << std::endl;
  std::cout << "light ray: " << lr.describe() << std::endl;
  std::cout << describe() << std::endl;

  // Compute dist between light ray and sphere center
  double t = (lr.dir_.dot(lr.origin_ - center_))/lr.dir_.norm();
  std::cout << "t: " << t << std::endl;
  Vector3 closestPoint = lr.origin_ - (lr.dir_ * t);
  std::cout << "closestPoint: " << closestPoint << std::endl;
  double dist = (closestPoint - center_).norm();
  std::cout << "dist: " << dist << std::endl;
  if(dist < radius_)
    return true;
  else
    return false;
}

std::ostream& operator<<(std::ostream& os, const Sphere& s)
{
  os << s.describe();
}
