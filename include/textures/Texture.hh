#pragma once

#include <iostream>
#include "Vector3.hh"

class Texture
{
public:
    Texture ();
    virtual ~Texture ();
    virtual Vector3RGB color(double x = 0, double y = 0) const = 0;
    virtual void switchColors() = 0;
};
