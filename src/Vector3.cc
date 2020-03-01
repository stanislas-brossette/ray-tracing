#include "Vector3.hh"

Vector3::Vector3(double x, double y, double z)
  : x_(x),
    y_(y),
    z_(z)
{
}

Vector3::~Vector3()
{
}

double Vector3::norm() const
{
  return std::sqrt(x_*x_ + y_*y_ + z_*z_);
}

void Vector3::normalize()
{
  double n = norm();
  x_ = x_ / n;
  y_ = y_ / n;
  z_ = z_ / n;
}

std::ostream& operator<<(std::ostream& os, const Vector3& v)
{
  os << "[" << v.x_ << "," << v.y_ << "," << v.z_ << "]";
}

Vector3RGB::Vector3RGB(int r, int g, int b)
  : r_(r),
    g_(g),
    b_(b)
{
  if(r_ < 0 or r_ > 255)
    std::cerr << "invalid R" << std::endl;
  if(g_ < 0 or g_ > 255)
    std::cerr << "invalid G" << std::endl;
  if(b_ < 0 or b_ > 255)
    std::cerr << "invalid B" << std::endl;
}

Vector3RGB::~Vector3RGB()
{
}

std::ostream& operator<<(std::ostream& os, const Vector3RGB& v)
{
  os << "[" << v.r_ << "," << v.g_ << "," << v.b_ << "]";
}
