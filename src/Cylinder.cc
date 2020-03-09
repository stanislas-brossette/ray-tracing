#include "Cylinder.hh"


Cylinder::Cylinder()
  : Geometry(),
    radius_(1.0)
{
}

Cylinder::Cylinder(const Frame3& f, double radius)
  : Geometry(f),
    radius_(radius)
{
}

Cylinder::~Cylinder()
{
}

std::string Cylinder::describe() const
{
    std::stringstream ss;
    ss << "=== Cylinder ===\n";
    ss << "center: " << f_.o_ << "\n";
    ss << "direction: " << f_.vz_ << "\n";
    ss << "radius: " << radius_ << "\n";
    return ss.str();
}

bool Cylinder::intersect(const LightRay& lr, Vector3& impactPoint, Vector3& normal, double& dist) const
{
    bool impact = false;

    Vector3 va = lr.origin_ - f_.o_ - f_.vz_ * (lr.origin_ - f_.o_).dot(f_.vz_);
    Vector3 vb = lr.dir_ - f_.vz_ * (lr.dir_.dot(f_.vz_));
    double a = vb.squaredNorm();
    double b = 2*va.dot(vb);
    double c = va.squaredNorm() - radius_*radius_;

    double d = b*b - 4*a*c;
    if (d >= 0)
    {
        impact = true;
    }
    else
    {
        return false;
    }
    double x0 = (-b+std::sqrt(d))/(2*a);
    double x1 = (-b-std::sqrt(d))/(2*a);

    if(x0 < 0 and x1 < 0)
    {
        return false;
    }
    else if(x0 <= x1 and x0 >= 0)
    {
        dist = x0;
    }
    else if(x1 <= x0 and x1 >= 0)
    {
        dist = x1;
    }
    else
    {
        return false;
    }

    impactPoint = lr.origin_ + lr.dir_ * dist;

    impactPoint = impactPoint;
    Vector3 impactPointInF = f_.pointFromWorld(impactPoint);
    normal = f_.vecToWorld(Vector3(impactPointInF.x_/radius_, impactPointInF.y_/radius_, 0));
    return impact;
}

bool Cylinder::isInHalfSpace(const Vector3& point, const Vector3& normal, double& cosAngle) const
{
    return false;
    // v = OC - r.normal
    //Vector3 v = f_.o_ - point;// - normal*radius_;
    //v.normalize();
    //cosAngle = v.dot(normal);
    //bool inHalfSpace = (cosAngle > 0);
    //return inHalfSpace;
}

