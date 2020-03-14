#include "ClosedCylinder.hh"

ClosedCylinder::ClosedCylinder()
  : Geometry(),
    radius_(1.0),
    cylinder_(),
    bottomPlane_(),
    topPlane_()
{
}

ClosedCylinder::ClosedCylinder(const Frame3& f, double radius, double length)
  : Geometry(f),
    radius_(radius),
    length_(length),
    cylinder_(f, radius, length)
{
    Frame3 fBP(f.o_ - f.vz_ * (length/2), f.vx_, f.vy_, f.vz_);
    bottomPlane_ = Plane(fBP);

    Frame3 fTP(f.o_ + f.vz_ * (length/2), f.vx_, f.vy_, f.vz_);
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
    Vector3 impactPointCyl;
    Vector3 normalCyl;
    double distCyl = 1e9;
    bool impactCyl = cylinder_.intersect(lr, impactPointCyl, normalCyl, distCyl);
    if(not impactCyl)
        distCyl = 1e9;

    Vector3 impactPointBP;
    Vector3 normalBP;
    double distBP = 1e9;
    bool impactBP = bottomPlane_.intersect(lr, impactPointBP, normalBP, distBP);
    impactBP = impactBP and ((impactPointBP - f_.o_ + (f_.vz_ * (length_/2))).squaredNorm() < radius_*radius_);
    if(not impactBP)
        distBP = 1e9;

    Vector3 impactPointTP;
    Vector3 normalTP;
    double distTP = 1e9;
    bool impactTP = topPlane_.intersect(lr, impactPointTP, normalTP, distTP);
    impactTP = impactTP and ((impactPointTP - f_.o_ - (f_.vz_ * (length_/2))).squaredNorm() < radius_*radius_);
    if(not impactTP)
        distTP = 1e9;

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

bool ClosedCylinder::isInHalfSpace(const Vector3& point, const Vector3& normal, double& cosAngle) const
{
    return false;
    // v = OC - r.normal
    //Vector3 v = f_.o_ - point;// - normal*radius_;
    //v.normalize();
    //cosAngle = v.dot(normal);
    //bool inHalfSpace = (cosAngle > 0);
    //return inHalfSpace;
}


std::string closedCylinderData::describe() const
{
    std::stringstream ss;
    ss << "=== ClosedCylinder ===\n";
    ss << geometryData::describe();
    ss << "radius: " << radius << "\n";
    ss << "length: " << length << "\n";
    return ss.str();
}
