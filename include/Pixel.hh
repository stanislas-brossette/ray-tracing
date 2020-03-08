#pragma once
#include <iostream>
#include <sstream>
#include <string>

class Pixel
{
public:
    Pixel();
    Pixel(int x, int y);
    Pixel(int x, int y, double a, double r, double g, double b);
    ~Pixel();
    Pixel operator+(const Pixel& p) const;
    void clamp();
    int x_;
    int y_;
    double a_;
    double r_;
    double g_;
    double b_;

    std::string describe() const;
};
