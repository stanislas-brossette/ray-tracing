#include <gtest/gtest.h>
#include <iostream>

#include "Vector3.hh"
#include "Frame3.hh"
#include "Camera.hh"

TEST(UnitTests, CameraTest)
{
    Frame3 f;
    double fovX = 120;
    double fovY = 100;
    double resX = 1000;
    Camera cam(f, fovX, fovY, resX);
    Pixel p;
    Vector3 dir;

    p.x_ = 0;
    p.y_ = 0;
    dir = cam.pixelToDir(p);
    std::cout << "pix: [" << p.x_ << ", " << p.y_ << "], dir: " << dir << std::endl;

    p.x_ = cam.resX_/2;
    p.y_ = cam.resY_/2;
    dir = cam.pixelToDir(p);
    ASSERT_TRUE(dir.isApprox(Vector3(0, 1, 0), 1e-9));
    std::cout << "pix: [" << p.x_ << ", " << p.y_ << "], dir: " << dir << std::endl;

    p.x_ = cam.resX_;
    p.y_ = cam.resY_;
    dir = cam.pixelToDir(p);
    std::cout << "pix: [" << p.x_ << ", " << p.y_ << "], dir: " << dir << std::endl;
}

