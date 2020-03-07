#pragma once
#include <iostream>
#include <cmath>

class Vector3
{
public:
    Vector3();
    Vector3 (double, double, double);
    virtual ~Vector3 ();

    double norm() const;
    void normalize();

    double dot(const Vector3&) const;

    Vector3 operator+(const Vector3&) const;
    Vector3 operator-(const Vector3&) const;
    Vector3 operator*(const double&) const;
    bool operator==(const Vector3&) const;
    bool isApprox(const Vector3&, double prec) const;

    Vector3 rotateX(double theta) const;
    Vector3 rotateY(double theta) const;
    Vector3 rotateZ(double theta) const;

    double x_;
    double y_;
    double z_;
    friend std::ostream& operator<<(std::ostream& os, const Vector3& v);
};

class Vector3RGB
{
public:
    Vector3RGB ();
    Vector3RGB (int, int, int);
    virtual ~Vector3RGB ();

    int r_;
    int g_;
    int b_;
    friend std::ostream& operator<<(std::ostream& os, const Vector3RGB& v);
};

