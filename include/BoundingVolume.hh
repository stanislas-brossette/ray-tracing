#pragma once
#include "Frame3.hh"
#include "LightRay.hh"

class BoundingVolume
{
public:
    BoundingVolume ();
    BoundingVolume (const Frame3& f, double radius);
    virtual ~BoundingVolume ();
    bool intersect(const LightRay& incident) const;

private:
    Frame3 f_;
    double radius_;
};
