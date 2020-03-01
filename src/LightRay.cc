#include "LightRay.hh"

LightRay::LightRay(Vector3 origin, Vector3 dir, double px, double py)
  : origin_(origin),
    dir_(dir),
    px_(px),
    py_(py)
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
  ss << "pixel: " << px_ << ", " << py_;
  return ss.str();
}
