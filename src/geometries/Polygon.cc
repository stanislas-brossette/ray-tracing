#include "geometries/Polygon.hh"

Polygon::Polygon()
  : Geometry()
{
}

Polygon::Polygon(const Frame3& f, const std::vector<Vector2>& p)
  : Geometry(f)
{
    PerformanceTracker::instance().incrementPolygons();
    points_ = p;
}

Polygon::Polygon(const Frame3& f, const std::vector<Vector2>& p, const std::vector<Vector3>& n)
    : Polygon(f,p)
{
    normals_ = n;
    // compute the terms used to compute the smooth normals
    // TODO: do that with eigen
    if(normals_.size() == 3)
    {
        double det = points_[1].x_ * points_[2].y_ - points_[2].x_ * points_[1].y_;
        a0 = ( points_[2].y_ * (normals_[1].x_ - normals_[0].x_) - points_[1].y_ * (normals_[2].x_ - normals_[0].x_))/det;
        a1 = (-points_[2].x_ * (normals_[1].x_ - normals_[0].x_) + points_[1].x_ * (normals_[2].x_ - normals_[0].x_))/det;
        b0 = ( points_[2].y_ * (normals_[1].y_ - normals_[0].y_) - points_[1].y_ * (normals_[2].y_ - normals_[0].y_))/det;
        b1 = (-points_[2].x_ * (normals_[1].y_ - normals_[0].y_) + points_[1].x_ * (normals_[2].y_ - normals_[0].y_))/det;
        c0 = ( points_[2].y_ * (normals_[1].z_ - normals_[0].z_) - points_[1].y_ * (normals_[2].z_ - normals_[0].z_))/det;
        c1 = (-points_[2].x_ * (normals_[1].z_ - normals_[0].z_) + points_[1].x_ * (normals_[2].z_ - normals_[0].z_))/det;
    }
}

Polygon::Polygon(PolygonData* pData)
  : Geometry(pData)
{
    PerformanceTracker::instance().incrementPolygons();
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
                      Vector3& normal, double& dist, bool verbose) const
{
    if(verbose)
    {
        std::cout << "Polygon::intersect" << std::endl;
        std::cout << "describe(): " << describe() << std::endl;
        std::cout << "lr: " << lr << std::endl;
    }
    // lr is supposed to be in world's frame
    PerformanceTracker::instance().incrementCallToIntersectPolygon();
    double verticalDist = (lr.origin_ - f_.o_).dot(f_.vz_);
    bool pointAbovePolygon = (verticalDist > 0);

    double vzScalDir = f_.vz_.dot(lr.dir_);
    bool lrSameDirNormal = (vzScalDir > 0);

    bool impact = false;
    int normalMultiplier = 1;
    if (pointAbovePolygon and not lrSameDirNormal)
    {
        impact = true;
    }
    else if (not pointAbovePolygon and lrSameDirNormal)
    {
        impact = true;
        normalMultiplier = -1;
    }
    else
    {
        if(verbose)
            std::cout << "No impact with polygons plane" << std::endl;
        return false;
    }

    dist = -verticalDist/vzScalDir;
    point = lr.origin_ + lr.dir_ * dist;
    Vector3 Pimpact = f_.pointFromWorld(point); // In planes frame
    Vector2 Pp(Pimpact.x_, Pimpact.y_); // In planes frame
    if(verbose)
    {
        std::cout << "impact point in world frame: " << point << std::endl;
        std::cout << "Pimpact in polygons frame: " << Pimpact << std::endl;
        std::cout << "Pp in polygons frame: " << Pp << std::endl;
    }

    for (size_t i = 0; i < points_.size(); i++)
    {
        size_t i1 = (i+1)%(points_.size());
        impact = impact and Pp.isRightOf(points_.at(i), points_.at(i1));
        if(not impact)
        {
            if(verbose)
                std::cout << "Pp not in triangle" << std::endl;
            return false;
        }
    }
    if(verbose)
        std::cout << "Pp in triangle" << std::endl;

    //Compute normal
    if( normals_.size() == points_.size())
    {
        if(verbose)
        {
            std::cout << "===CALCUL NORMAL FROM 3 NORMALS===" << std::endl;
            std::cout << "normals_[0]: " << normals_[0] << std::endl;
            std::cout << "normals_[1]: " << normals_[1] << std::endl;
            std::cout << "normals_[2]: " << normals_[2] << std::endl;
            std::cout << "f_: " << f_ << std::endl;
        }
        normal = Vector3(0,0,0);

        normal.x_ = normals_[0].x_ + a0 * Pp.x_ + a1 * Pp.y_;
        normal.y_ = normals_[0].y_ + b0 * Pp.x_ + b1 * Pp.y_;
        normal.z_ = normals_[0].z_ + c0 * Pp.x_ + c1 * Pp.y_;

        if(verbose)
            std::cout << "normal: " << normal << std::endl;

        normal *= -normalMultiplier;
        if(verbose)
        {
            std::cout << "normal: " << normal << std::endl;
            std::cout << "==================================" << std::endl;
        }
    }
    else
    {
      normal = f_.vz_;
      normal *= normalMultiplier;
    }


    return impact;
}

bool Polygon::isInHalfSpace(const Vector3& point, const Vector3& normal, const Vector3& specDir, double& cosAngleDiffuse, double& cosAnglePhong) const
{
    return false;
}

std::string PolygonData::describe() const
{
    std::stringstream ss;
    ss << "=== PolygonData ===\n";
    ss << GeometryData::describe();
    ss << "points: ";
    for (size_t i = 0; i < points.size()-1; i++)
    {
        ss << points.at(i) << ",";
    }
    ss << points.at(points.size()-1);
    return ss.str();
}

