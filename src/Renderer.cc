#include "Renderer.hh"

std::string RenderData::describe() const
{
    std::stringstream ss;
    ss << "=== RenderData ===\n";
    ss << "nPixPerRender: " << nPixPerRender << "\n";
    ss << "percentLightRay: " << percentLightRay << "\n";
    return ss.str();
}

Renderer::Renderer()
    : nPixPerRender_(0),
    percentLightRay_(0)
{
}

Renderer::Renderer(const RenderData& rData)
    : nPixPerRender_(rData.nPixPerRender),
    percentLightRay_(rData.percentLightRay)
{
}

Renderer::~Renderer()
{
}

void Renderer::renderParallel(const Scene& sc, Window& win, const std::string& s)
{
    //win.clear();
    int iter = 0;
    int nLightRay = win.nPixels()*percentLightRay_/100;
    std::chrono::steady_clock::time_point begin = std::chrono::steady_clock::now();
    PerformanceTracker::instance().startRenderTimer();
    std::cout << "0%" << std::endl;
    while(iter < nLightRay)
    {
        size_t nPixToRender = nPixPerRender_;
        if(iter+nPixPerRender_ > win.nPixels())
            nPixToRender = win.nPixels() - iter;
        std::vector<Pixel> pixs(nPixToRender);
        sc.renderParallel(pixs, nPixToRender, iter);
        win.addPixels(pixs);
        win.render();
        iter+=nPixPerRender_;
        PerformanceTracker::instance().currentRenderTimer();
        std::cout << (int)(100*(double)iter/(double)nLightRay) << "%\t" << PerformanceTracker::instance().currentRenderTime() << std::endl;
    }
    PerformanceTracker::instance().endRenderTimer();
}

