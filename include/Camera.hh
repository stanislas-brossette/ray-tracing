#pragma once
#include <iostream>
#include <sstream>
#include <string>
#include <cstdlib>

#include "Vector3.hh"
#include "Frame3.hh"
#include "LightRay.hh"
#include "Pixel.hh"

struct camData
{
    int resX = 0;
    double fovX = 0;
    double fovY = 0;
    Vector3 pos = Vector3(0,0,0);
    Vector3 rotAxis = Vector3(0,0,0);
    double rotAngle = 0;
    std::string describe() const;
};

class Camera
{
public:
    Camera ();
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
