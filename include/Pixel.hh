#pragma once
#include <iostream>
#include <sstream>
#include <string>

#include "Vector3.hh"

class Pixel
{
public:
    Pixel();
    Pixel(int x, int y);
    Pixel(int x, int y, double a, double r, double g, double b);
    ~Pixel();
    Pixel operator+(const Pixel& p) const;
    Pixel operator*(const double& d) const;
    void clamp();
    void setColor(double a, double r, double g, double b);
    void setColor(double a, const Vector3RGB c);
    int x_;
    int y_;
    double a_;
    double r_;
    double g_;
    double b_;

    std::string describe() const;
    friend std::ostream& operator<<(std::ostream& os, const Pixel& p);
};
