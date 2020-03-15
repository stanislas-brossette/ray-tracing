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
    AmbiantLight (double alpha, const Vector3RGB& v);
    virtual ~AmbiantLight ();
    std::string describe() const;

    double alpha_;
    Vector3RGB rgb_;
};
