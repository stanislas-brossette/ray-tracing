#include "geometries/ClosedCylinder.hh"

ClosedCylinder::ClosedCylinder()
  : Geometry(),
    radius_(1.0),
    cylinder_(),
    bottomPlane_(),
    topPlane_()
{
}

ClosedCylinder::ClosedCylinder(ClosedCylinderData* cData)
  : Geometry(cData),
    radius_(cData->radius),
    length_(cData->length),
    cylinder_(f_, radius_, length_)
{
    initPlanes();
    bv_ = BoundingVolume(f_, std::sqrt(length_*length_ + radius_*radius_));
}

ClosedCylinder::ClosedCylinder(const Frame3& f, double radius, double length)
  : Geometry(f),
    radius_(radius),
    length_(length),
    cylinder_(f_, radius, length)
{
    initPlanes();
    bv_ = BoundingVolume(f_, std::sqrt(length_*length_ + radius_*radius_));
}

void ClosedCylinder::initPlanes()
{
    Frame3 fBP(f_.o_ - f_.vz_ * (length_/2), f_.vx_, f_.vy_, f_.vz_);
    bottomPlane_ = Plane(fBP);

    Frame3 fTP(f_.o_ + f_.vz_ * (length_/2), f_.vx_, f_.vy_, f_.vz_);
    topPlane_ = Plane(fTP);
}

ClosedCylinder::~ClosedCylinder()
{
}

std::string ClosedCylinder::describe() const
{
    std::stringstream ss;
    ss << "=== ClosedCylinder ===\n";
    ss << "center: " << f_.o_ << "\n";
    ss << "direction: " << f_.vz_ << "\n";
    ss << "radius: " << radius_ << "\n";
    ss << "length: " << length_ << "\n";
    return ss.str();
}

bool ClosedCylinder::intersect(const LightRay& lr, Vector3& impactPoint, Vector3& normal, double& dist) const
{
    if(not bv_.intersect(lr))
        return false;

    Vector3 impactPointCyl;
    Vector3 normalCyl;
    double distCyl = INFINITY_d();
    bool impactCyl = cylinder_.intersect(lr, impactPointCyl, normalCyl, distCyl);
    if(not impactCyl)
        distCyl = INFINITY_d();

    Vector3 impactPointBP;
    Vector3 normalBP;
    double distBP = INFINITY_d();
    bool impactBP = bottomPlane_.intersect(lr, impactPointBP, normalBP, distBP);
    impactBP = impactBP and ((impactPointBP - f_.o_ + (f_.vz_ * (length_/2))).squaredNorm() < radius_*radius_);
    if(not impactBP)
        distBP = INFINITY_d();

    Vector3 impactPointTP;
    Vector3 normalTP;
    double distTP = INFINITY_d();
    bool impactTP = topPlane_.intersect(lr, impactPointTP, normalTP, distTP);
    impactTP = impactTP and ((impactPointTP - f_.o_ - (f_.vz_ * (length_/2))).squaredNorm() < radius_*radius_);
    if(not impactTP)
        distTP = INFINITY_d();

    if( not( impactTP or impactBP or impactCyl))
        return false;
    else if(impactCyl and distCyl <= distTP and distCyl <= distBP)
    {
        impactPoint = impactPointCyl;
        normal = normalCyl;
        dist = distCyl;
        return true;
    }
    else if(impactTP and distTP <= distCyl and distTP <= distBP)
    {
        impactPoint = impactPointTP;
        normal = normalTP;
        dist = distTP;
        return true;
    }
    else if(impactBP and distBP <= distCyl and distBP <= distTP)
    {
        impactPoint = impactPointBP;
        normal = normalBP;
        dist = distBP;
        return true;
    }
    else
        return false;

}

bool ClosedCylinder::isInHalfSpace(const Vector3& point, const Vector3& normal, const Vector3& specDir, double& cosAngleDiffuse, double& cosAnglePhong) const
{
    return false;
}


std::string ClosedCylinderData::describe() const
{
    std::stringstream ss;
    ss << "=== ClosedCylinder ===\n";
    ss << GeometryData::describe();
    ss << "radius: " << radius << "\n";
    ss << "length: " << length << "\n";
    return ss.str();
}
