#include "Vector3.hh"

Vector2::Vector2()
  : x_(0),
    y_(0)
{
}

Vector2::Vector2(double x, double y)
  : x_(x),
    y_(y)
{
}

Vector2::~Vector2()
{
}

bool Vector2::isRightOf(const Vector2& p0, const Vector2& p1) const
{
    //std::cout << "IsRightOf" << std::endl;
    double prodVec = (x_ - p0.x_)*(p1.y_ - p0.y_) - (y_ - p0.y_)*(p1.x_ - p0.x_);
    //std::cout << "prodVec: " << prodVec << std::endl;
    return (prodVec > 0);
}

std::ostream& operator<<(std::ostream& os, const Vector2& v)
{
    os << "[" << v.x_ << "," << v.y_ << "]";
}

Vector3::Vector3()
  : x_(0),
    y_(0),
    z_(0)
{
}

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

double Vector3::squaredNorm() const
{
    return x_*x_ + y_*y_ + z_*z_;
}

Vector3 Vector3::normalize()
{
    double n = norm();
    x_ = x_ / n;
    y_ = y_ / n;
    z_ = z_ / n;
    return *this;
}

double Vector3::dot(const Vector3& v) const
{
    return (x_*v.x_ + y_*v.y_ + z_*v.z_);
}

Vector3 Vector3::cross(const Vector3& v) const
{
    Vector3 out;
    out.x_ = y_ * v.z_ - z_ * v.y_;
    out.y_ = z_ * v.x_ - x_ * v.z_;
    out.z_ = x_ * v.z_ - z_ * v.x_;
    return out;
}

Vector3 Vector3::operator+(const Vector3& v) const
{
    Vector3 res(x_+v.x_, y_+v.y_, z_+v.z_);
    return res;
}

Vector3 Vector3::operator-(const Vector3& v) const
{
    Vector3 res(x_-v.x_, y_-v.y_, z_-v.z_);
    return res;
}

Vector3 Vector3::operator*(const double& d) const
{
    Vector3 res(d*x_, d*y_, d*z_);
    return res;
}

bool Vector3::operator==(const Vector3& v) const
{
    if(v.x_ == x_ and v.y_ == y_ and v.z_ == z_)
        return true;
    else
        return false;
}

bool Vector3::isApprox(const Vector3& v, double prec) const
{
    if((std::abs(v.x_ - x_) <= prec) and
       (std::abs(v.y_ - y_) <= prec) and
       (std::abs(v.z_ - z_) <= prec))
        return true;
    else
        return false;
}

Vector3 Vector3::rotateX(double theta) const
{
    double theta_rad = theta * M_PI / 180;
    double s = std::sin(theta_rad);
    double c = std::cos(theta_rad);
    Vector3 rotV(x_, c*y_-s*z_, s*y_+c*z_);
    return rotV;
}

Vector3 Vector3::rotateY(double theta) const
{
    double theta_rad = theta * M_PI / 180;
    double s = std::sin(theta_rad);
    double c = std::cos(theta_rad);
    Vector3 rotV(c*x_+s*z_, y_, -s*x_+c*z_);
    return rotV;
}

Vector3 Vector3::rotateZ(double theta) const
{
    double theta_rad = theta * M_PI / 180;
    double s = std::sin(theta_rad);
    double c = std::cos(theta_rad);
    Vector3 rotV(c*x_-s*y_, s*x_+c*y_, z_);
    return rotV;
}

void Vector3::addNoise(double rugosity)
{
    int randSeed = 10000;
    double randX = rugosity*(double(std::rand()%randSeed)/(randSeed/2)-1);
    double randY = rugosity*(double(std::rand()%randSeed)/(randSeed/2)-1);
    double randZ = rugosity*(double(std::rand()%randSeed)/(randSeed/2)-1);
    x_ += randX;
    y_ += randY;
    z_ += randZ;
    normalize();
}

Vector3 Vector3::symmetrize(const Vector3& n) const
{
    double c1 = -n.dot(*this);
    Vector3 res(n*(2*c1) + *this);
    return res;
}

bool Vector3::refract(Vector3& T, const Vector3& N, double n1, double n2) const
{
    double n1sn2 = n1/n2;
    double c1 = -N.dot(*this);
    double c2 = 1 - std::pow(n1sn2,2)*(1-std::pow(c1,2));
    if(c2 >= 0)
    {
        c2 = std::sqrt(c2);
        T = *this * n1sn2 + N * (n1sn2*c1 - c2);
        return true;
    }
    else
        return false;
}

std::ostream& operator<<(std::ostream& os, const Vector3& v)
{
    os << "[" << v.x_ << "," << v.y_ << "," << v.z_ << "]";
}

Vector3RGB::Vector3RGB()
  : r_(0.0),
    g_(0.0),
    b_(0.0)
{
}

Vector3RGB::Vector3RGB(double c)
  : Vector3RGB(c, c, c)
{
}

Vector3RGB::Vector3RGB(double r, double g, double b)
  : r_(r),
    g_(g),
    b_(b)
{
    if(r_ < 0 or r_ > 1.0)
        std::cerr << "invalid R: " << r_ << std::endl;
    if(g_ < 0 or g_ > 1.0)
        std::cerr << "invalid G: " << g_ << std::endl;
    if(b_ < 0 or b_ > 1.0)
        std::cerr << "invalid B: " << b_ << std::endl;
}

Vector3RGB::~Vector3RGB()
{
}

std::ostream& operator<<(std::ostream& os, const Vector3RGB& v)
{
    os << "[" << v.r_ << "," << v.g_ << "," << v.b_ << "]";
}

Vector3RGB& Vector3RGB::operator=(const Vector3RGB& c)
{
    r_ = c.r_;
    g_ = c.g_;
    b_ = c.b_;
    return *this;
}

Vector3RGB Vector3RGB::operator+(const Vector3RGB& c) const
{
    return Vector3RGB(c.r_ + r_, c.g_ + g_, c.g_ + g_);
}

Vector3RGB Vector3RGB::operator*(const Vector3RGB& c) const
{
    return Vector3RGB(c.r_ * r_, c.g_ * g_, c.g_ * g_);
}

Vector3RGB Vector3RGB::operator*(double d) const
{
    return Vector3RGB(r_*d, g_*d, g_*d);
}

void Vector3RGB::clamp(double min, double max)
{
    if(r_ <= min)
        r_ = min;
    if(r_ >= max)
        r_ = max;

    if(g_ <= min)
        g_ = min;
    if(g_ >= max)
        g_ = max;

    if(b_ <= min)
        b_ = min;
    if(b_ >= max)
        b_ = max;
}

void Vector3RGB::applyGammaCorrection(double exposure, double gamma)
{
    r_ = std::pow(r_*exposure, gamma);
    g_ = std::pow(g_*exposure, gamma);
    b_ = std::pow(b_*exposure, gamma);
}
