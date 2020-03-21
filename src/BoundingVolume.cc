#include "BoundingVolume.hh"

BoundingVolume::BoundingVolume()
    : f_(),
    radius_(0)
{
}

BoundingVolume::BoundingVolume(const Frame3& f, double radius)
    : f_(f),
    radius_(radius)
{
}

BoundingVolume::~BoundingVolume()
{
}

bool BoundingVolume::intersect(const LightRay& lr) const
{
    Vector3 OmSo = lr.origin_ - f_.o_;
    double dist = (OmSo - lr.dir_ * (lr.dir_.dot(OmSo))).squaredNorm() - radius_*radius_;
    return (dist < 0);
}
