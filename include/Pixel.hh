#pragma once
#include <iostream>
#include <sstream>
#include <string>

class Pixel
{
public:
    Pixel();
    Pixel(int x, int y);
    Pixel(int x, int y, int a, int r, int g, int b);
    ~Pixel();
    void clamp();
    int x_;
    int y_;
    int a_;
    int r_;
    int g_;
    int b_;

    std::string describe() const;
};
