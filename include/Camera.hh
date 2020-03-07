#pragma once
#include <iostream>
#include <sstream>
#include <string>
#include <cstdlib>

#include "Vector3.hh"
#include "Frame3.hh"
#include "LightRay.hh"
#include "Pixel.hh"

class Camera
{
public:
    Camera (const Frame3& frame, double fovX, double fovY, int resX);
    virtual ~Camera ();
    std::string describe() const;
    void castRandomRay(LightRay& lr, Pixel& px) const;
    Vector3 pixelToDir(const Pixel& px) const;

    Frame3 frame_;
    double fovX_;
    double fovY_;
    int resX_;
    int resY_;

private:
    double focalDist_;
};
