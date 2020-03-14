#pragma once
#include <iostream>
#include <sstream>

#include "Vector3.hh"

struct MaterialData
{
    Vector3RGB color = Vector3RGB(0,0,0);
    double rugosity = 0.0;
    double refraction = 0.0;
    double reflectiveness = 0.0;
    bool lightEmitter = false;
    double lightIntensity = 0.0;
    std::string describe() const;
};

class Material
{
public:
    Material ();
    Material (const MaterialData& mData);
    Material (const Vector3RGB& color, double rugosity, double refIndex, double reflectiveness, bool lightEmitter, double lightIntensity);
    std::string describe() const;
    virtual ~Material ();

    Vector3RGB color_;
    double rugosity_;
    double refractionIndex_;
    double reflectiveness_;
    bool lightEmitter_;
    double lightIntensity_;
};
