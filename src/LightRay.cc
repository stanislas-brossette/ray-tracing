#include "LightRay.hh"

LightRay::LightRay()
  : origin_(),
    dir_()
{
}

LightRay::LightRay(Vector3 origin, Vector3 dir)
  : origin_(origin),
    dir_(dir)
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
    ss << "dir: " << dir_;
    return ss.str();
}
