#include "Sphere.hh"

Sphere::Sphere(const Frame& f, double radius)
  : f_(f),
    radius_(radius)
{
}

Sphere::~Sphere()
{
}

std::string describe() const
{
  std::stringstream ss;
  ss << "=== Sphere ===\n";
  ss << "center: " << f_.o_ << "\n";
  ss << "radius: " << radius_ << "\n";
  return ss.str();
}

bool intersect(const LightRay& lr, Vector3& point, Vector3& normal) const
{
  // Compute dist between light ray and sphere center
  double t = (lr.dir_.dot(lr.origin_ - f_.o_))/lr.dir_.norm();
  Vector3 closestPoint = lr.origin_ - (lr.dir_ * t);
  double orthogDist = (closestPoint - f_.o_).norm();
}

