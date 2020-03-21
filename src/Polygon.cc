#include "Polygon.hh"

Polygon::Polygon()
  : Geometry()
{
}

Polygon::Polygon(const Frame3& f)
  : Geometry(f)
{
}

Polygon::Polygon(PolygonData* pData)
  : Geometry(pData)
{
    points_.resize(pData->points.size());
    for (size_t i = 0; i < points_.size(); i++)
    {
        points_[i] = pData->points[i];
    }
}

Polygon::~Polygon()
{
}

std::string Polygon::describe() const
{
    std::stringstream ss;
    ss << "=== Polygon ===\n";
    ss << "origin: " << f_.o_ << "\n";
    ss << "normal: " << f_.vz_ << "\n";
    ss << "points: ";
    for (size_t i = 0; i < points_.size()-1; i++)
    {
        ss << points_.at(i) << ",";
    }
    ss << points_.at(points_.size()-1);

    return ss.str();
}

bool Polygon::intersect(const LightRay& lr, Vector3& point,
                      Vector3& normal, double& dist) const
{
    bool pointAbovePolygon = ((lr.origin_ - f_.o_).dot(f_.vz_) > 0);
    bool lrSameDirNormal = (f_.vz_.dot(lr.dir_) > 0);
    bool impact = false;
    if (pointAbovePolygon and not lrSameDirNormal)
    {
        impact = true;
        normal = f_.vz_;
    }
    else if (not pointAbovePolygon and lrSameDirNormal)
    {
        impact = true;
        normal = f_.vz_ * -1.0;
    }
    else
    {
        return false;
    }

    dist = (f_.o_ - lr.origin_).dot(normal) / lr.dir_.dot(normal);
    Vector3 Pimpact = lr.origin_ + (lr.dir_*dist) - f_.o_; // In planes frame
    Vector2 Pp(Pimpact.dot(f_.vx_), Pimpact.dot(f_.vy_)); // In planes frame

    for (size_t i = 0; i < points_.size(); i++)
    {
        size_t i1 = (i+1)%(points_.size());
        impact = impact and Pp.isRightOf(points_.at(i), points_.at(i1));
        if(not impact)
            return false;
    }

    point = lr.origin_ + lr.dir_ * dist;
    return impact;
}

bool Polygon::isInHalfSpace(const Vector3& point, const Vector3& normal,
                          double& cosAngle) const
{
    return false;
}

std::string PolygonData::describe() const
{
    std::stringstream ss;
    ss << "=== Polygon ===\n";
    ss << GeometryData::describe();
    ss << "points: ";
    for (size_t i = 0; i < points.size()-1; i++)
    {
        ss << points.at(i) << ",";
    }
    ss << points.at(points.size()-1);
    return ss.str();
}

