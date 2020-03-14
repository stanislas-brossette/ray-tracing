#include "Renderer.hh"

std::string RenderData::describe() const
{
    std::stringstream ss;
    ss << "=== RenderData ===\n";
    ss << "nPixPerRender: " << nPixPerRender << "\n";
    ss << "nLightRay: " << nLightRay << "\n";
    return ss.str();
}

Renderer::Renderer()
    : nPixPerRender_(0),
    nLightRay_(0)
{
}

Renderer::Renderer(const RenderData& rData)
    : nPixPerRender_(rData.nPixPerRender),
    nLightRay_(rData.nLightRay)
{
}

Renderer::~Renderer()
{
}

