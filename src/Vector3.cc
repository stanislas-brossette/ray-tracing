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

Vector3 Vector3::vectorial(const Vector3& v) const
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

Vector3 Vector3::refract(const Vector3& N, double n1, double n2) const
{
    double n1sn2 = n1/n2;
    double c1 = -N.dot(*this);
    double c2 = std::sqrt(1 - std::pow(n1sn2,2)*(1-std::pow(c1,2)));
    Vector3 T = *this * n1sn2 + N * (n1sn2*c1 - c2);
    return T;
}

std::ostream& operator<<(std::ostream& os, const Vector3& v)
{
    os << "[" << v.x_ << "," << v.y_ << "," << v.z_ << "]";
}

Vector3RGB::Vector3RGB()
  : r_(255),
    g_(255),
    b_(255)
{
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
