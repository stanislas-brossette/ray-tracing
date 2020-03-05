#include "Plane.hh"

Plane::Plane()
  : Geometry()
{
}

Plane::Plane(const Frame3& f)
  : Geometry(f)
{
}

Plane::~Plane()
{
}

std::string Plane::describe() const
{
  std::stringstream ss;
  ss << "=== Plane ===\n";
  ss << f_.describe();
  return ss.str();
}

bool Plane::intersect(const LightRay& lr, Vector3& point, Vector3& normal, double& dist) const
{
  bool pointAbovePlane = ((lr.origin_ - f_.o_).dot(f_.vz_) > 0);
  bool lrSameDirNormal = (f_.vz_.dot(lr.dir_) > 0);
  bool impact = false;
  if (pointAbovePlane and not lrSameDirNormal)
  {
    impact = true;
    normal = f_.vz_;
  }
  else if (not pointAbovePlane and lrSameDirNormal)
  {
    impact = true;
    normal = f_.vz_ * -1.0;
  }

  if(impact)
  {
    double l = (f_.o_ - lr.origin_).dot(normal)/(lr.dir_.dot(normal));
    point = lr.origin_ + lr.dir_ * l;
  }
  return impact;
}

bool Plane::isInHalfSpace(const Vector3& point, const Vector3& normal, double& cosAngle) const
{
  return false;
}
