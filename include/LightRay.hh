#pragma once
#include <iostream>
#include <sstream>
#include <string>
#include "Vector3.hh"

class LightRay
{
public:
    LightRay ();
    LightRay (Vector3 origin, Vector3 dir, double refractiveIndex_ = 1.0);
    virtual ~LightRay ();
    std::string describe() const;
    friend std::ostream& operator<<(std::ostream& os, const LightRay& lr);

    Vector3 origin_;
    Vector3 dir_;
    double refractiveIndex_;
};
