#include "Frame3.hh"

Frame3::Frame3()
  : o_(0, 0, 0),
    vx_(1, 0, 0),
    vy_(0, 1, 0),
    vz_(0, 0, 1)
{
}

Frame3::Frame3(const Vector3& o, const Vector3& vx, const Vector3& vy, const Vector3& vz)
  : o_(o),
    vx_(vx),
    vy_(vy),
    vz_(vz)
{
}

Frame3::~Frame3()
{
}

void Frame3::translate(const Vector3& v)
{
    o_ = o_ + v;
}

void Frame3::translate(double x, double y, double z)
{
    Vector3 v(x, y, z);
    translate(v);
}

void Frame3::translateLocal(double x, double y, double z)
{
    Vector3 v(x, y, z);
    Vector3 v_w = vecToWorld(v);
    translate(v_w);
}

void Frame3::setOriginPos(double x, double y, double z)
{
    o_.x_ = x;
    o_.y_ = y;
    o_.z_ = z;
}

void Frame3::rotate(const Vector3& axis, double theta)
{
    //normalize axis
    Vector3 u = axis * (1/axis.norm());
    double theta_rad = theta * M_PI / 180;
    double s = std::sin(theta_rad);
    double c = std::cos(theta_rad);
    Vector3 row0 = Vector3(u.x_*u.x_*(1-c)+c     , u.x_*u.y_*(1-c)-u.z_*s, u.x_*u.z_*(1-c)+u.y_*s);
    Vector3 row1 = Vector3(u.x_*u.y_*(1-c)+u.z_*s, u.y_*u.y_*(1-c)+c     , u.y_*u.z_*(1-c)-u.x_*s);
    Vector3 row2 = Vector3(u.x_*u.z_*(1-c)-u.y_*s, u.y_*u.z_*(1-c)+u.x_*s, u.z_*u.z_*(1-c)+c);

    Vector3 tmpV;

    tmpV.x_ = row0.dot(vx_);
    tmpV.y_ = row1.dot(vx_);
    tmpV.z_ = row2.dot(vx_);
    vx_ = tmpV;

    tmpV.x_ = row0.dot(vy_);
    tmpV.y_ = row1.dot(vy_);
    tmpV.z_ = row2.dot(vy_);
    vy_ = tmpV;

    tmpV.x_ = row0.dot(vz_);
    tmpV.y_ = row1.dot(vz_);
    tmpV.z_ = row2.dot(vz_);
    vz_ = tmpV;
}

void Frame3::pitch(double theta)
{
    double theta_rad = theta * M_PI / 180;
    double s = std::sin(theta_rad);
    double c = std::cos(theta_rad);
    Vector3 vyTmp = vy_;
    Vector3 vzTmp = vz_;
    vy_ = (vyTmp * c) + (vzTmp * s);
    vz_ = (vzTmp * c) - (vyTmp * s);
}
void Frame3::roll(double theta)
{
    double theta_rad = theta * M_PI / 180;
    double s = std::sin(theta_rad);
    double c = std::cos(theta_rad);
    Vector3 vxTmp = vx_;
    Vector3 vzTmp = vz_;
    vz_ = (vzTmp * c) + (vxTmp * s);
    vx_ = (vxTmp * c) - (vzTmp * s);
}
void Frame3::yaw(double theta)
{
    double theta_rad = theta * M_PI / 180;
    double s = std::sin(theta_rad);
    double c = std::cos(theta_rad);
    Vector3 vxTmp = vx_;
    Vector3 vyTmp = vy_;
    vx_ = (vxTmp * c) + (vyTmp * s);
    vy_ = (vyTmp * c) - (vxTmp * s);
}

bool Frame3::operator==(const Frame3& f)
{
    if (o_ == f.o_ and
        vx_ == f.vx_ and
        vy_ == f.vy_ and
        vz_ == f.vz_)
        return true;
    else
        return false;
}

bool Frame3::isApprox(const Frame3& f, double prec)
{
    if (o_.isApprox(f.o_, prec) and
        vx_.isApprox(f.vx_, prec) and
        vy_.isApprox(f.vy_, prec) and
        vz_.isApprox(f.vz_, prec))
        return true;
    else
        return false;
}

Vector3 Frame3::vecToWorld(const Vector3& v) const
{
    Vector3 res((vx_ * v.x_) + (vy_ * v.y_) + (vz_ * v.z_));
    return res;
}

Vector3 Frame3::pointToWorld(const Vector3& v) const
{
    Vector3 res(o_ + (vx_ * v.x_) + (vy_ * v.y_) + (vz_ * v.z_));
    return res;
}

Vector3 Frame3::vecFromWorld(const Vector3& v) const
{
    Vector3 res(v.dot(vx_), v.dot(vy_), v.dot(vz_));
    return res;
}

Vector3 Frame3::pointFromWorld(const Vector3& p) const
{
    Vector3 p2 = p - o_;
    Vector3 res(p2.dot(vx_), p2.dot(vy_), p2.dot(vz_));
    return res;
}

std::string Frame3::describe() const
{
    std::stringstream ss;
    ss << "=== Frame3 ===\n";
    ss << "o: " << o_ << "\n";
    ss << "vx: " << vx_ << "\n";
    ss << "vy: " << vy_ << "\n";
    ss << "vz: " << vz_;
    return ss.str();
}

std::ostream& operator<<(std::ostream& os, const Frame3& f)
{
    os << f.describe();
    return os;
}
