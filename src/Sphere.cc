#include "Sphere.hh"

Sphere::Sphere(const Vector3& c, double r)
  : center_(c),
    radius_(r)
{
}

Sphere::~Sphere()
{
}

std::string Sphere::describe() const
{
  std::stringstream ss;
  ss << "=== Sphere ===\n";
  ss << "c: " << center_ << "\n";
  ss << "r: " << radius_;
  return ss.str();
}

std::ostream& operator<<(std::ostream& os, const Sphere& s)
{
  os << s.describe();
}
