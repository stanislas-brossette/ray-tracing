#pragma once
#include <string>
#include <sstream>

struct renderData
{
    int nPixPerRender = 0;
    int nLightRay = 0;
    std::string describe() const;
};

class Renderer
{
public:
    Renderer ();
    virtual ~Renderer ();

private:
};
