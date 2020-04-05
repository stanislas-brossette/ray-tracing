#include "geometries/Cylinder.hh"

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
    bs_ = BoundingSphere(f_, std::sqrt(length_*length_ + radius_*radius_));
}

Cylinder::Cylinder(CylinderData* cData)
  : Geometry(cData),
    radius_(cData->radius),
    length_(cData->length)
{
    bs_ = BoundingSphere(f_, std::sqrt(length_*length_ + radius_*radius_));
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
    //Don't use bounding volume for infinite cylinder
    if(length_ < INFINITY_d() and not bs_.intersect(lr))
        return false;

    bool impact = false;

    Vector3 va = lr.origin_ - f_.o_ - f_.vz_ * (lr.origin_ - f_.o_).dot(f_.vz_);
    Vector3 vb = lr.dir_ - f_.vz_ * (lr.dir_.dot(f_.vz_));
    double a = vb.squaredNorm();
    double b = 2*va.dot(vb);
    double c = va.squaredNorm() - radius_*radius_;

    double x0, x1;
    if(not solve2ndOrderEq(a, b, c, x0, x1))
        return false;

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
            return true;
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
                return true;
            }
            else
                return false;
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
            return true;
        }
        else
            return false;
    }
    else
        return false;
}

bool Cylinder::isInHalfSpace(const Vector3& point, const Vector3& normal, const Vector3& specDir, double& cosAngleDiffuse, double& cosAnglePhong) const
{
    return false;
}

std::string CylinderData::describe() const
{
    std::stringstream ss;
    ss << "=== Cylinder ===\n";
    ss << GeometryData::describe();
    ss << "radius: " << radius << "\n";
    ss << "length: " << length << "\n";
    return ss.str();
}
