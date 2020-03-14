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

Sphere::Sphere(SphereData* sData)
  : Geometry(sData),
    radius_(sData->radius)
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
    Vector3 va = lr.origin_ - f_.o_;

    double a = lr.dir_.squaredNorm();
    double b = 2*va.dot(lr.dir_);
    double c = va.squaredNorm() - radius_*radius_;
    double x0, x1;
    if(not solve2ndOrderEq(a, b, c, x0, x1))
        return false;

    if(x0 < 0 and x1 < 0)
        return false;
    else if(x0 >= 0)
    {
        dist = x0;
        impactPoint = lr.origin_ + lr.dir_ * dist;
        normal = impactPoint - f_.o_;
        normal.normalize();
        return true;
    }
    else if(x1 >= 0)
    {
        dist = x1;
        impactPoint = lr.origin_ + lr.dir_ * dist;
        normal = f_.o_ - impactPoint;
        normal.normalize();
        return true;
    }
    else
        return false;
}

bool Sphere::isInHalfSpace(const Vector3& point, const Vector3& normal, double& cosAngle) const
{
    // v = OC - r.normal
    Vector3 v = f_.o_ - point;// - normal*radius_;
    v.normalize();
    cosAngle = v.dot(normal);
    bool inHalfSpace = (cosAngle > 0);
    return inHalfSpace;
}

std::string SphereData::describe() const
{
    std::stringstream ss;
    ss << "=== SphereData ===\n";
    ss << GeometryData::describe();
    ss << "radius: " << radius << "\n";
    return ss.str();
}
