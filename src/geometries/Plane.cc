#include "geometries/Plane.hh"

Plane::Plane()
  : Geometry()
{
}

Plane::Plane(const Frame3& f)
  : Geometry(f)
{
}

Plane::Plane(PlaneData* pData)
  : Geometry(pData)
{
}

Plane::~Plane()
{
}

std::string Plane::describe() const
{
    std::stringstream ss;
    ss << "=== Plane ===\n";
    ss << "origin: " << f_.o_ << "\n";
    ss << "normal: " << f_.vz_ << "\n";

    return ss.str();
}

bool Plane::intersect(const LightRay& lr, Vector3& point,
                      Vector3& normal, double& dist) const
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
        dist = std::abs((f_.o_ - lr.origin_).dot(normal)/(lr.dir_.dot(normal)));
        point = lr.origin_ + lr.dir_ * dist;
    }
    return impact;
}

bool Plane::isInHalfSpace(const Vector3& point, const Vector3& normal,
                          double& cosAngle) const
{
    return false;
}

std::string PlaneData::describe() const
{
    std::stringstream ss;
    ss << "=== Plane ===\n";
    ss << GeometryData::describe();
    return ss.str();
}
