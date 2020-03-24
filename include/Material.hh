#pragma once
#include <iostream>
#include <sstream>

#include "Vector3.hh"
#include "dataStructures.hh"

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
    double refractiveIndex_;
    double reflectiveness_;
    bool lightEmitter_;
    double lightIntensity_;
};
