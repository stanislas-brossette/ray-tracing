#include "Cylinder.hh"


Cylinder::Cylinder()
  : Geometry(),
    radius_(1.0)
{
}

Cylinder::Cylinder(const Frame3& f, double radius, double length)
  : Geometry(f),
    radius_(radius),
    length_(length)
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
    ss << "length: " << length_ << "\n";
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
    if (d < 0) //No solution, the light ray does not impact the infinite cylinder
    {
        return false;
    }

    // a is always positive, so x0 < x1
    double x0 = (-b-std::sqrt(d))/(2*a);
    double x1 = (-b+std::sqrt(d))/(2*a);

    if(x0 < 0 and x1 < 0)
    {
        return false;
    }
    else if(x0 >= 0) //x0 touched first
    {
        dist = x0;
        impactPoint = lr.origin_ + lr.dir_ * dist;
        Vector3 impactPointInF = f_.pointFromWorld(impactPoint);
        if(impactPointInF.z_ <= length_/2 and impactPointInF.z_ >= -length_/2)
        {
            normal = f_.vecToWorld(Vector3(impactPointInF.x_/radius_, impactPointInF.y_/radius_, 0));
            impact = true;
            return impact;
        }
        else
        {
            dist = x1;
            impactPoint = lr.origin_ + lr.dir_ * dist;
            Vector3 impactPointInF = f_.pointFromWorld(impactPoint);
            if(impactPointInF.z_ <= length_/2 and impactPointInF.z_ >= -length_/2)
            {
                normal = f_.vecToWorld(Vector3(impactPointInF.x_/radius_, impactPointInF.y_/radius_, 0));
                normal = normal * -1;
                impact = true;
                return impact;
            }
            else
            {
                impact = false;
                return impact;
            }
        }
    }
    else if(x1 >= 0)
    {
        dist = x1;
        impactPoint = lr.origin_ + lr.dir_ * dist;
        Vector3 impactPointInF = f_.pointFromWorld(impactPoint);
        if(impactPointInF.z_ <= length_/2 and impactPointInF.z_ >= -length_/2)
        {
            normal = f_.vecToWorld(Vector3(impactPointInF.x_/radius_, impactPointInF.y_/radius_, 0));
            normal = normal * -1;
            impact = true;
            return impact;
        }
    }
    else
    {
        return false;
    }
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

