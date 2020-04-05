#include "geometries/BoundingSphere.hh"

BoundingSphere::BoundingSphere()
    : f_(),
    radius_(0)
{
}

BoundingSphere::BoundingSphere(const Frame3& f, double radius)
    : f_(f),
    radius_(radius)
{
}

BoundingSphere::~BoundingSphere()
{
}

bool BoundingSphere::intersect(const LightRay& lr) const
{
    Vector3 OmSo = lr.origin_ - f_.o_;
    double dist = (OmSo - lr.dir_ * (lr.dir_.dot(OmSo))).squaredNorm() - radius_*radius_;
    return (dist < 0);
}
