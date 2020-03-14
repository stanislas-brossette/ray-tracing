#pragma once

struct renderData
{
    int nPixPerRender = 0;
    int nLightRay = 0;
};

class Renderer
{
public:
    Renderer ();
    virtual ~Renderer ();

private:
};
