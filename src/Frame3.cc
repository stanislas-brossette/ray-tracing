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
