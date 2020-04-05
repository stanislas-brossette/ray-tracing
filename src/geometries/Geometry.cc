#include "geometries/Geometry.hh"

Geometry::Geometry()
  : f_(),
    bs_()
{
}

Geometry::Geometry(const Frame3& f)
  : f_(f),
    bs_(f)
{
}

Geometry::Geometry(GeometryData* gData)
  : f_(),
    bs_(f_)
{
    f_.translate(gData->pos);
    f_.rotate(gData->rotAxis, gData->rotAngle);
}

Geometry::~Geometry()
{
}

std::string GeometryData::describe() const
{
    std::stringstream ss;
    ss << "type: " << type << "\n";
    ss << "pos: " << pos << "\n";
    ss << "rotAxis: " << rotAxis << "\n";
    ss << "rotAngle: " << rotAngle << "\n";
    return ss.str();
}
