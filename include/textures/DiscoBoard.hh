#pragma once
#include <cmath>
#include <vector>
#include <math.h>
#include <algorithm>
#include "textures/Texture.hh"

class DiscoBoard : public Texture
{
public:
    DiscoBoard (double squareSizeX, double squareSizeY);
    virtual ~DiscoBoard () = default;
    Vector3RGB color(double x, double y) const;
    void switchColors();

private:
    std::vector<Vector3RGB> colors_;
    double squareSizeX_;
    double squareSizeY_;
    double borderSize_;
    int switchedColors_;
};

