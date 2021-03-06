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
    friend std::ostream& operator<<(std::ostream& os, const Camera& c);
    void castRandomRay(LightRay& lr, Pixel& px) const;
    void castRayAt(const int& pX, const int& pY, LightRay& lr, Pixel& px) const;
    void castOrderedRay(LightRay& lr, Pixel& px, size_t index) const;
    Vector3 pixelToDir(const Pixel& px) const;
    int nPixels() const {return resX_*resY_;};
    void changeResolution(int resX, int resY);

    void rotateToTarget();
    void moveToDistanceToTarget(double d);

    Frame3 frame_;
    double fov_;
    Vector3 target_;
    Vector3 upGuide_;
    double aspectRatio_;
    int resX_;
    int resY_;
    double screenWidth_;
    double screenHeight_;

private:
    double focalDist_;
    std::vector<std::pair<int, int>> allPixels_;
    void initAllPixelsVec();
};
