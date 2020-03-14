#include "Renderer.hh"

std::string renderData::describe() const
{
    std::stringstream ss;
    ss << "=== renderData ===\n";
    ss << "nPixPerRender: " << nPixPerRender << "\n";
    ss << "nLightRay: " << nLightRay << "\n";
    return ss.str();
}

Renderer::Renderer()
{
}

Renderer::~Renderer()
{
}

