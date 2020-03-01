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

std::ostream& operator<<(std::ostream& os, const Sphere& s)
{
  os << s.describe();
}
