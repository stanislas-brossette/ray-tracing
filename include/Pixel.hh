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
    Pixel(int x, int y, double r, double g, double b);
    ~Pixel();

    Pixel operator+(const Pixel& p) const;
    Pixel operator*(const double& d) const;
    Pixel operator*(const Pixel& p) const;

    Pixel& operator+=(const Pixel& p);
    Pixel& operator*=(const double& d);
    Pixel& operator*=(const Pixel& p);
    Pixel& operator*=(const Vector3RGB& c);

    void clamp();
    void applyGammaCorrection(double exposure, double gamma);
    void setColor(double intensity, double r, double g, double b);
    void setColor(double intensity, const Vector3RGB c);
    int x_;
    int y_;
    Vector3RGB c_;
    double r() const {return c_.r_;};
    double g() const {return c_.g_;};
    double b() const {return c_.b_;};

    std::string describe() const;
    friend std::ostream& operator<<(std::ostream& os, const Pixel& p);
};
