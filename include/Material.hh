#pragma once
#include <iostream>

#include "Vector3.hh"

class Material
{
public:
    Material ();
    Material (const Vector3RGB color, double rugosity, double refIndex, bool lightEmitter, double lightIntensity);  
    virtual ~Material ();

    Vector3RGB color_;
    double rugosity_;
    double refractionIndex_;
    bool lightEmitter_;
    double lightIntensity_;
};
