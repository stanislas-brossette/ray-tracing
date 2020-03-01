#include "Sphere.hh"

Sphere::Sphere(const Vector3& center, double radius, const Vector3RGB& color)
  : center_(center),
    radius_(radius),
    color_(color)
{
}

Sphere::~Sphere()
{
}

std::string Sphere::describe() const
{
  std::stringstream ss;
  ss << "=== Sphere ===\n";
  ss << "center: " << center_ << "\n";
  ss << "radius: " << radius_ << "\n";
  ss << "color: " << color_ << "\n";
  return ss.str();
}

Pixel Sphere::intersect(const LightRay& lr, LightRay& lr2, bool& contact, double& dist) const
{
  // Compute dist between light ray and sphere center
  double t = (lr.dir_.dot(lr.origin_ - center_))/lr.dir_.norm();
  Vector3 closestPoint = lr.origin_ - (lr.dir_ * t);
  double orthogDist = (closestPoint - center_).norm();
  if(orthogDist < radius_ and orthogDist > 0)
  {
    contact = true;
    double circleRadius = std::sqrt(radius_*radius_ - orthogDist*orthogDist);
    dist = std::abs(t) - circleRadius;
    Vector3 contactPoint = lr.origin_ + lr.dir_ * dist;
    Vector3 normal = contactPoint - center_;
    normal.normalize();
    lr2.origin_ = contactPoint;
    lr2.dir_ = lr.dir_ - normal * (2 * lr.dir_.dot(normal));

    //Randomize lr2
    double randX = 0.0005*(double(std::rand()%1000)-500);
    double randY = 0.0005*(double(std::rand()%1000)-500);
    double randZ = 0.0005*(double(std::rand()%1000)-500);
    lr2.dir_.x_ += randX;
    lr2.dir_.y_ += randY;
    lr2.dir_.z_ += randZ;
    lr2.dir_.normalize();

    return Pixel(lr.px_, lr.py_, 1, color_.r_, color_.g_, color_.b_);
  }
  else
  {
    contact = false;
    return Pixel(lr.px_, lr.py_, 1, 0,0,0);
  }
}

std::ostream& operator<<(std::ostream& os, const Sphere& s)
{
  os << s.describe();
}
