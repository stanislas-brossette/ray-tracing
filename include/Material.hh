#pragma once
#include <iostream>
#include <sstream>

#include "Vector3.hh"
#include "dataStructures.hh"
#include "textures/Texture.hh"
#include "textures/SolidColor.hh"
#include "textures/CheckerBoard.hh"
#include "textures/DiscoBoard.hh"
#include "textures/Image.hh"

class Material
{
public:
    Material ();
    Material (MaterialData* mData);
    std::string describe() const;
    friend std::ostream& operator<<(std::ostream& os, const Material& m);
    virtual ~Material ();

    Texture* texture_;
    int specularExponent_;
    double specularGain_;
    double rugosity_;
    double refractiveIndex_;
    double absorption_;
    double reflectiveness_;
    bool lightEmitter_;
    double lightIntensity_;
};
