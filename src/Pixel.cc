#include "Pixel.hh"

Pixel::Pixel ()
  : x_(0),
    y_(0),
    a_(0),
    c_(0, 0, 0)
{
}

Pixel::Pixel (int x, int y)
  : x_(x),
    y_(y),
    a_(0),
    c_(0, 0, 0)
{
}

Pixel::Pixel (int x, int y, double a, double r, double g, double b)
  : x_(x),
    y_(y),
    a_(a),
    c_(r, g, b)
{
}

Pixel::~Pixel()
{
}

void Pixel::setColor(double a, double r, double g, double b)
{
    a_ = a;
    c_ = Vector3RGB(r, g, b);
}
void Pixel::setColor(double a, const Vector3RGB c)
{
    a_ = a;
    c_ = c;
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
    if(a_ < 0.0)
        a_ = 0.0;
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
    pRes.a_ = a_ + p.a_;
    //double ratio = a_/(a_ + p.a_);
    //double ratioP = p.a_/(a_ + p.a_);
    //pRes.r_ = ratio*r_ + ratioP*p.r_;
    //pRes.g_ = ratio*g_ + ratioP*p.g_;
    //pRes.b_ = ratio*b_ + ratioP*p.b_;
    pRes.c_ = c_ + p.c_;
    return pRes;
}

Pixel Pixel::operator*(const Pixel& p) const
{
    Pixel pRes;
    pRes.x_ = x_;
    pRes.y_ = y_;
    pRes.a_ = a_;
    pRes.c_ = c_ * p.c_;
    return pRes;
}

Pixel Pixel::operator*(const double& d) const
{
    Pixel pRes;
    pRes.x_ = x_;
    pRes.y_ = y_;
    pRes.a_ = a_ * d;
    pRes.c_ = c_ * d;
    return pRes;
}

std::ostream& operator<<(std::ostream& os, const Pixel& p)
{
    os << p.describe();
    return os;
}
