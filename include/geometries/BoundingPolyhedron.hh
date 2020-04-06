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
    BoundingPolyhedron (const Frame3& f, const std::vector<Vector3>& planeNormals);
    BoundingPolyhedron (const Frame3& f, const std::vector<Vector3>& planeNormals, const std::vector<double>& pMin, const std::vector<double>& pMax);
    ~BoundingPolyhedron ();
    bool intersect(const LightRay& incident, Vector3& point, Vector3& normal, double& dist) const;
    bool contains(const Vector3& point);
    void extendBy(const Vector3& point);
    std::string describe() const;

    Frame3 f_;
    std::vector<Vector3> planeNormals_;
    std::vector<double> pMin_;
    std::vector<double> pMax_;
};
