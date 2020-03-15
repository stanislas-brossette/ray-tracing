#pragma once
#include <string>
#include <sstream>

#include "dataStructures.hh"
#include "Window.hh"
#include "Scene.hh"

class Renderer
{
public:
    Renderer ();
    Renderer (const RenderData& rData);
    void renderParallel(const Scene& sc, Window& win, const std::string& s = "");
    void renderSerial(const Scene& sc, Window& win, const std::string& s = "");
    virtual ~Renderer ();
    int nPixPerRender_ = 0;
    int nLightRay_ = 0;

private:
};
