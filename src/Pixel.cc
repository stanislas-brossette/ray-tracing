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

Pixel::Pixel (int x, int y, double a, double r, double g, double b)
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

void Pixel::setColor(double a, double r, double g, double b)
{
    a_ = a;
    r_ = r;
    g_ = g;
    b_ = b;
}
void Pixel::setColor(double a, const Vector3RGB c)
{
    a_ = a;
    r_ = c.r_;
    g_ = c.g_;
    b_ = c.b_;
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
    if(a_ < 0)
        a_ = 0;
    if(r_ < 0)
        r_ = 0;
    if(g_ < 0)
        g_ = 0;
    if(b_ < 0)
        b_ = 0;
}

Pixel Pixel::operator+(const Pixel& p) const
{
    Pixel pRes;
    pRes.x_ = x_;
    pRes.y_ = y_;
    pRes.a_ = a_ + p.a_;
    double ratio = a_/(a_ + p.a_);
    double ratioP = p.a_/(a_ + p.a_);
    pRes.r_ = ratio*r_ + ratioP*p.r_;
    pRes.g_ = ratio*g_ + ratioP*p.g_;
    pRes.b_ = ratio*b_ + ratioP*p.b_;
    return pRes;
}

Pixel Pixel::operator*(const double& d) const
{
    Pixel pRes;
    pRes.x_ = x_;
    pRes.y_ = y_;
    pRes.a_ = d*a_;
    pRes.r_ = r_;
    pRes.g_ = g_;
    pRes.b_ = b_;
    return pRes;
}
