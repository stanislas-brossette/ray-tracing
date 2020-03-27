#pragma once
#include <iostream>
#include <sstream>
#include <string>

#include "Vector3.hh"
#include "dataStructures.hh"

class AmbiantLight
{
public:
    AmbiantLight ();
    AmbiantLight (const AmbiantData& aData);
    AmbiantLight (double intensity, const Vector3RGB& color);
    virtual ~AmbiantLight ();
    std::string describe() const;
    friend std::ostream& operator<<(std::ostream& os, const AmbiantLight& a);

    double intensity_;
    Vector3RGB color_;
};
