#pragma once
#include <vector>
#include <string>
#include <sstream>

#include "Frame3.hh"
#include "LightRay.hh"
#include "utils.hh"

class BoundingPolyhedron
{
public:
    BoundingPolyhedron () = default;
    BoundingPolyhedron (const Frame3& f);
    ~BoundingPolyhedron ();
    bool intersect(const LightRay& incident) const;
    void extendBy(const Vector3& point);
    std::string describe() const;

private:
    Frame3 f_;
    std::vector<Vector3> planeNormals_;
    std::vector<double> pMin_;
    std::vector<double> pMax_;
};
