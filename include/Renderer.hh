#pragma once
#include <string>
#include <sstream>

struct RenderData
{
    int nPixPerRender = 0;
    int nLightRay = 0;
    std::string describe() const;
};

class Renderer
{
public:
    Renderer ();
    Renderer (const RenderData& rData);
    virtual ~Renderer ();
    int nPixPerRender_ = 0;
    int nLightRay_ = 0;

private:
};
