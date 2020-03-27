#include "LightRay.hh"

LightRay::LightRay()
  : origin_(),
    dir_(),
    refractiveIndex_(1.0)
{
}

LightRay::LightRay(Vector3 origin, Vector3 dir, double refractiveIndex)
  : origin_(origin),
    dir_(dir),
    refractiveIndex_(refractiveIndex)
{
}

LightRay::~LightRay()
{
}

std::string LightRay::describe() const
{
    std::stringstream ss;
    ss << "=== LightRay ===\n";
    ss << "origin: " << origin_ << "\n";
    ss << "dir: " << dir_ << "\n";
    ss << "refractiveIndex: " << refractiveIndex_;
    return ss.str();
}

std::ostream& operator<<(std::ostream& os, const LightRay& lr)
{
    os << lr.describe();
    return os;
}
