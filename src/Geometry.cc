#include "Geometry.hh"

Geometry::Geometry()
  : f_()
{
}

Geometry::Geometry(const Frame3& f)
  : f_(f)
{
}

Geometry::~Geometry()
{
}

std::string geometryData::describe() const
{
    std::stringstream ss;
    ss << "type: " << type << "\n";
    ss << "pos: " << pos << "\n";
    ss << "rotAxis: " << rotAxis << "\n";
    ss << "rotAngle: " << rotAngle << "\n";
    return ss.str();
}
