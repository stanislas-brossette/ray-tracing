#pragma once
#include <iostream>
#include <sstream>
#include <string>
#include <cstdlib>
#include <vector>
#include <utility>
#include <algorithm>

#include "dataStructures.hh"
#include "Vector3.hh"
#include "Frame3.hh"
#include "LightRay.hh"
#include "Pixel.hh"

class Camera
{
public:
    Camera ();
    Camera (const CamData& cData);
    Camera (const Frame3& frame, double fovX, double fovY, int resX);
    virtual ~Camera ();
    std::string describe() const;
    void castRandomRay(LightRay& lr, Pixel& px) const;
    void castOrderedRay(LightRay& lr, Pixel& px, size_t index) const;
    Vector3 pixelToDir(const Pixel& px) const;
    int nPixels() const {return resX_*resY_;};

    Frame3 frame_;
    double fovX_;
    double fovY_;
    int resX_;
    int resY_;

private:
    double focalDist_;
    std::vector<std::pair<int, int>> allPixels_;
    void initAllPixelsVec();
};
