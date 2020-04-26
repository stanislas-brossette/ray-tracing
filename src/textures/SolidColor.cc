#include "textures/SolidColor.hh"

SolidColor::SolidColor(const Vector3RGB& c)
    : Texture(),
    color_(c)
{
}

SolidColor::~SolidColor()
{
}

Vector3RGB SolidColor::color(double x = 0, double y = 0) const
{
    return color_;
}

void SolidColor::switchColors()
{
    return;
}
