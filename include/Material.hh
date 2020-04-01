#pragma once
#include <iostream>
#include <sstream>

#include "Vector3.hh"
#include "dataStructures.hh"
#include "textures/Texture.hh"
#include "textures/SolidColor.hh"

class Material
{
public:
    Material ();
    Material (MaterialData* mData);
    //Material (const Vector3RGB& color, double rugosity, double refIndex, double reflectiveness, bool lightEmitter, double lightIntensity);
    std::string describe() const;
    friend std::ostream& operator<<(std::ostream& os, const Material& m);
    virtual ~Material ();

    Texture* texture_;
    double rugosity_;
    double refractiveIndex_;
    double reflectiveness_;
    bool lightEmitter_;
    double lightIntensity_;
};
