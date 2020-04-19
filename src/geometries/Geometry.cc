#include "geometries/Geometry.hh"

Geometry::Geometry()
  : f_(),
    bs_(),
    simplifiedRender_(false)
{
}

Geometry::Geometry(const Frame3& f)
  : f_(f),
    bs_(f),
    simplifiedRender_(false)
{
}

Geometry::Geometry(GeometryData* gData)
  : f_(),
    bs_(f_),
    simplifiedRender_(false)
{
    f_.translate(gData->pos);
    if(gData->usingAngleAxis)
        f_.rotate(gData->rotAxis, gData->rotAngle);
    else
    {
        f_.vx_ = gData->vx.normalize();
        f_.vy_ = gData->vy.normalize();
        f_.vz_ = gData->vz.normalize();
    }
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
