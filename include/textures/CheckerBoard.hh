#pragma once
#include <cmath>
#include "textures/Texture.hh"

class CheckerBoard : public Texture
{
public:
    CheckerBoard (const Vector3RGB& color0, const Vector3RGB color1, double squareSizeX, double squareSizeY);
    virtual ~CheckerBoard () = default;
    Vector3RGB color(double x, double y) const;

private:
    Vector3RGB color0_;
    Vector3RGB color1_;
    double squareSizeX_;
    double squareSizeY_;
};
