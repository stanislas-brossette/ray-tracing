#pragma once

#include "textures/Texture.hh"

class SolidColor : public Texture
{
public:
    SolidColor (const Vector3RGB& c);
    virtual ~SolidColor ();
    Vector3RGB color(double x, double y) const;
    void switchColors();
    void setColor(const Vector3RGB& c);

private:
    Vector3RGB color_;
};
