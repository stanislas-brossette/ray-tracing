#include "Pixel.hh"

Pixel::Pixel (int x, int y, int a, int r, int g, int b)
  : x_(x),
    y_(y),
    a_(a),
    r_(r),
    g_(g),
    b_(b)
{
}

Pixel::~Pixel()
{
}

std::string Pixel::describe() const
{
  std::stringstream ss;
  ss << "Pixel(x:" << x_ << ",y:" << y_ << ",a:" << a_ << ",r:" << r_ << ",g:" << g_ << ",b:" << b_ << ")";
  return ss.str();
}
