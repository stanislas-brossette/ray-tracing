#include "Sphere.hh"

Sphere::Sphere()
  : Geometry(),
    radius_(1.0)
{
}

Sphere::Sphere(const Frame3& f, double radius)
  : Geometry(f),
    radius_(radius)
{
}

Sphere::~Sphere()
{
}

std::string Sphere::describe() const
{
  std::stringstream ss;
  ss << "=== Sphere ===\n";
  ss << "center: " << f_.o_ << "\n";
  ss << "radius: " << radius_ << "\n";
  return ss.str();
}

bool Sphere::intersect(const LightRay& lr, Vector3& impactPoint, Vector3& normal, double& dist) const
{
  // Compute dist between light ray and sphere center
  double lrDirNorm = lr.dir_.norm();
  if(std::abs(lrDirNorm - 1) > 1e-9)
    std::cout << "Warning: the direction of lr is not normalized" << std::endl;
  double t = -(lr.dir_.dot(lr.origin_ - f_.o_))/lrDirNorm;
  Vector3 closestPoint = lr.origin_ + (lr.dir_ * t);
  double orthogDist = (closestPoint - f_.o_).norm();
  bool impact = false;
  if(orthogDist <= radius_ && t > 0)
  {
    double circleRadius = std::sqrt(radius_*radius_ - orthogDist*orthogDist);
    dist = t - circleRadius;
    impactPoint = lr.origin_ + lr.dir_ * dist;
    normal = impactPoint - f_.o_;
    normal.normalize();
    impact = true;
  }
  return impact;
}

bool Sphere::isInHalfSpace(const Vector3& point, const Vector3& normal, double& cosAngle) const
{
    // v = OC - r.normal
    Vector3 v = f_.o_ - point - normal*radius_;
    v.normalize();
    cosAngle = v.dot(normal);
    bool inHalfSpace = (cosAngle > 0);
    return inHalfSpace;
}
