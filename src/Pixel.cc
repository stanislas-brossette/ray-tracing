#include "Pixel.hh"

Pixel::Pixel ()
  : x_(0),
    y_(0),
    c_(0, 0, 0)
{
}

Pixel::Pixel (int x, int y)
  : x_(x),
    y_(y),
    c_(0, 0, 0)
{
}

Pixel::Pixel (int x, int y, double a, double r, double g, double b)
  : x_(x),
    y_(y),
    c_(r, g, b)
{
}

Pixel::~Pixel()
{
}

void Pixel::setColor(double a, double r, double g, double b)
{
    c_ = Vector3RGB(a*r, a*g, a*b);
}
void Pixel::setColor(double a, const Vector3RGB c)
{
    c_ = c * a;
}

std::string Pixel::describe() const
{
    std::stringstream ss;
    ss << "Pixel(x:" << x_ << ",y:" << y_ << ",a:" << a_;
    ss << ",r:" << r() << ",g:" << g() << ",b:" << b() << ")";
    return ss.str();
}

void Pixel::clamp()
{
    c_.clamp();
}

void Pixel::applyGammaCorrection(double exposure, double gamma)
{
    c_.applyGammaCorrection(exposure, gamma);
}

Pixel Pixel::operator+(const Pixel& p) const
{
    if(p.a_ == 0)
        return *this;
    Pixel pRes;
    pRes.x_ = x_;
    pRes.y_ = y_;
    pRes.c_ = c_ + p.c_;
    return pRes;
}

Pixel Pixel::operator*(const Pixel& p) const
{
    Pixel pRes;
    pRes.x_ = x_;
    pRes.y_ = y_;
    pRes.c_ = c_ * p.c_;
    return pRes;
}

Pixel Pixel::operator*(const double& d) const
{
    Pixel pRes;
    pRes.x_ = x_;
    pRes.y_ = y_;
    pRes.c_ = c_ * d;
    return pRes;
}

Pixel& Pixel::operator+=(const Pixel& p)
{
    c_ += p.c_;
    return *this;
}

Pixel& Pixel::operator*=(const Pixel& p)
{
    c_ *= p.c_;
    return *this;
}

Pixel& Pixel::operator*=(const double& d)
{
    c_ *= d;
    return *this;
}

std::ostream& operator<<(std::ostream& os, const Pixel& p)
{
    os << p.describe();
    return os;
}
