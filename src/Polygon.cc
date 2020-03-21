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

    return ss.str();
}

bool Polygon::intersect(const LightRay& lr, Vector3& point,
                      Vector3& normal, double& dist) const
{
    //std::cout << "Polygon::intersect" << std::endl;
    //std::cout << "(lr.origin_ - f_.o_).dot(f_.vz_): " << (lr.origin_ - f_.o_).dot(f_.vz_) << std::endl;
    //std::cout << "f_.vz_.dot(lr.dir_): " << f_.vz_.dot(lr.dir_) << std::endl;
    bool pointAbovePolygon = ((lr.origin_ - f_.o_).dot(f_.vz_) > 0);
    bool lrSameDirNormal = (f_.vz_.dot(lr.dir_) > 0);
    bool impact = false;
    if (pointAbovePolygon and not lrSameDirNormal)
    {
        //std::cout << "impact of correct side" << std::endl;
        impact = true;
        normal = f_.vz_;
        double l = (f_.o_ - lr.origin_).dot(f_.vz_) / lr.dir_.dot(f_.vz_);
        //std::cout << "l: " << l << std::endl;
        Vector3 Pimpact = lr.origin_ + (lr.dir_*l) - f_.o_;
        //std::cout << "Pimpact: " << Pimpact << std::endl;
        Vector2 Pp(Pimpact.dot(f_.vx_), Pimpact.dot(f_.vy_));
        //std::cout << "Pp: " << Pp << std::endl;
        //std::cout << "Pp.x_: " << Pp.x_ << std::endl;
        //std::cout << "Pp.y_: " << Pp.y_ << std::endl;
        Vector2 P0(0.1, 0.1);
        Vector2 P1(0.0, -1.0);
        Vector2 P2(-1.0, 0.0);
        bool b0 = Pp.isRightOf(P0, P1);
        bool b1 = Pp.isRightOf(P1, P2);
        bool b2 = Pp.isRightOf(P2, P0);
        if(b0 and b1 and b2)
            impact = true;
        else
            impact = false;
    }
    else if (not pointAbovePolygon and lrSameDirNormal)
    {
        impact = true;
        normal = f_.vz_ * -1.0;
        double l = (f_.o_ - lr.origin_).dot(normal) / lr.dir_.dot(normal);
        //std::cout << "l: " << l << std::endl;
        Vector3 Pimpact = lr.origin_ + (lr.dir_*l) - f_.o_;
        //std::cout << "Pimpact: " << Pimpact << std::endl;
        Vector2 Pp(Pimpact.dot(f_.vx_), Pimpact.dot(f_.vy_));
        //std::cout << "Pp: " << Pp << std::endl;
        //std::cout << "Pp.x_: " << Pp.x_ << std::endl;
        //std::cout << "Pp.y_: " << Pp.y_ << std::endl;
        Vector2 P0(0.1, 0.1);
        Vector2 P1(0.0, -1.0);
        Vector2 P2(-1.0, 0.0);
        bool b0 = Pp.isRightOf(P0, P1);
        bool b1 = Pp.isRightOf(P1, P2);
        bool b2 = Pp.isRightOf(P2, P0);
        if(b0 and b1 and b2)
            impact = true;
        else
            impact = false;
        //bool right0 = Pp.isRightOf(P0, P1);
        //bool right1 = Pp.isRightOf(P1, P2);
        //bool right1 = Pp.isRightOf(P1, P2);
    }

    if(impact)
    {
        dist = std::abs((f_.o_ - lr.origin_).dot(normal)/(lr.dir_.dot(normal)));
        point = lr.origin_ + lr.dir_ * dist;
    }
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
    return ss.str();
}

