#pragma once
#include "Frame3.hh"
#include "LightRay.hh"

class BoundingSphere
{
public:
    BoundingSphere ();
    BoundingSphere (const Frame3& f, double radius = 0);
    virtual ~BoundingSphere ();
    bool intersect(const LightRay& incident, bool verbose = false) const;

private:
    Frame3 f_;
    double radius_;
};
