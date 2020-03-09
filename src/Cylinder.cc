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
    // lr vectors will probs need to be rotated to get in the cylinders frame
    double a = lr.dir_.x_*lr.dir_.x_ + lr.dir_.y_*lr.dir_.y_;
    double b = lr.origin_.x_*lr.dir_.x_ + lr.origin_.y_*lr.dir_.y_;
    double c = lr.origin_.x_*lr.origin_.x_ + lr.origin_.y_*lr.origin_.y_ - radius_*radius_;
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
    double x1 = (b+std::sqrt(d))/(2*a);

    Vector3 P0 = lr.origin_ + lr.dir_ * x0;
    Vector3 P1 = lr.origin_ + lr.dir_ * x1;
    double dist0 = P0.norm();
    double dist1 = P1.norm();
    if(dist0 <= dist1)
    {
        dist = dist0;
        impactPoint = P0;
    }
    else
    {
        dist = dist1;
        impactPoint = P1;
    }
    normal = Vector3(radius_ * impactPoint.x_, radius_ * impactPoint.y_, 0);
    return impact;
    

    // Compute dist between light ray and Cylinder center
    //double lrDirNorm = lr.dir_.norm();
    //if(std::abs(lrDirNorm - 1) > 1e-9)
    //    std::cout << "Warning: the direction of lr is not normalized" << std::endl;
    //double t = -(lr.dir_.dot(lr.origin_ - f_.o_))/lrDirNorm;
    //Vector3 closestPoint = lr.origin_ + (lr.dir_ * t);
    //double orthogDist = (closestPoint - f_.o_).norm();
    //bool impact = false;
    //if(orthogDist <= radius_ && t > 0)
    //{
    //    double circleRadius = std::sqrt(radius_*radius_ - orthogDist*orthogDist);
    //    dist = t - circleRadius;
    //    impactPoint = lr.origin_ + lr.dir_ * dist;
    //    normal = impactPoint - f_.o_;
    //    normal.normalize();
    //    impact = true;
    //}
    //return impact;
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

