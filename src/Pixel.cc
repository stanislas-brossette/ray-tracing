#include "Pixel.hh"

Pixel::Pixel ()
  : x_(0),
    y_(0),
    a_(0),
    r_(0),
    g_(0),
    b_(0)
{
}

Pixel::Pixel (int x, int y)
  : x_(x),
    y_(y),
    a_(0),
    r_(0),
    g_(0),
    b_(0)
{
}

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
    ss << "Pixel(x:" << x_ << ",y:" << y_ << ",a:" << a_;
    ss << ",r:" << r_ << ",g:" << g_ << ",b:" << b_ << ")";
    return ss.str();
}

void Pixel::clamp()
{
    if(a_ > 255)
        a_ = 255;
    if(r_ > 255)
        r_ = 255;
    if(g_ > 255)
        g_ = 255;
    if(b_ > 255)
        b_ = 255;
}
