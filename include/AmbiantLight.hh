#pragma once
#include <iostream>
#include <sstream>
#include <string>

#include "Vector3.hh"

struct ambiantData
{
    Vector3RGB color = Vector3RGB(0, 0, 0);
    double intensity = 0;
    std::string describe() const;
};

class AmbiantLight
{
public:
    AmbiantLight ();
    AmbiantLight (double alpha, const Vector3RGB& v);
    virtual ~AmbiantLight ();
    std::string describe() const;

    double alpha_;
    Vector3RGB rgb_;
};
