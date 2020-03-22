#pragma once
#include <iostream>
#include <cmath>

class Vector2
{
public:
    Vector2();
    Vector2 (double, double);
    virtual ~Vector2 ();
    double x_;
    double y_;
    bool isRightOf(const Vector2& p0, const Vector2& p1) const;
    friend std::ostream& operator<<(std::ostream& os, const Vector2& v);
};

class Vector3
{
public:
    Vector3();
    Vector3 (double, double, double);
    virtual ~Vector3 ();

    double norm() const;
    double squaredNorm() const;
    Vector3 normalize();

    double dot(const Vector3&) const;
    Vector3 vectorial(const Vector3&) const;

    Vector3 operator+(const Vector3&) const;
    Vector3 operator-(const Vector3&) const;
    Vector3 operator*(const double&) const;
    bool operator==(const Vector3&) const;
    bool isApprox(const Vector3&, double prec) const;

    Vector3 rotateX(double theta) const;
    Vector3 rotateY(double theta) const;
    Vector3 rotateZ(double theta) const;

    Vector3 symmetrize(const Vector3& n) const;
    void addNoise(double val);

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

