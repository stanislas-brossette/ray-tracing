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
    win.clear();
    int iter = 0;
    int nLightRay = win.nPixels()*percentLightRay_/100;
    std::chrono::steady_clock::time_point begin = std::chrono::steady_clock::now();
    while(iter < nLightRay)
    {
        std::chrono::steady_clock::time_point beginBatch = std::chrono::steady_clock::now();
        size_t nPixToRender = nPixPerRender_;
        if(iter+nPixPerRender_ > win.nPixels())
            nPixToRender = win.nPixels() - iter;
        std::vector<Pixel> pixs(nPixToRender);
        sc.renderParallel(pixs, nPixToRender, iter);
        win.addPixels(pixs);
        win.render();
        iter+=nPixPerRender_;
        std::chrono::steady_clock::time_point endBatch = std::chrono::steady_clock::now();
        std::cout << "duration batch (" << nPixToRender << " points): " << std::chrono::duration_cast<std::chrono::milliseconds>(endBatch - beginBatch).count() << "[ms]" << std::endl;
    }
    std::chrono::steady_clock::time_point end = std::chrono::steady_clock::now();
    std::cout << "duration Parallel = " << std::chrono::duration_cast<std::chrono::milliseconds>(end - begin).count() << "[ms]" << std::endl;
    std::cout << s;
}

void Renderer::renderSerial(const Scene& sc, Window& win, const std::string& s)
{
    win.clear();
    int iter = 0;
    int nLightRay = win.nPixels()*percentLightRay_/100;
    std::chrono::steady_clock::time_point begin = std::chrono::steady_clock::now();
    while(iter < nLightRay)
    {
        std::vector<Pixel> pixs(nPixPerRender_);
        sc.renderSerial(pixs, nPixPerRender_, iter);
        win.addPixels(pixs);
        win.render();
        iter+=nPixPerRender_;
    }
    std::chrono::steady_clock::time_point end = std::chrono::steady_clock::now();
    std::cout << "duration Serial = " << std::chrono::duration_cast<std::chrono::microseconds>(end - begin).count() << "[µs]" << std::endl;
    std::cout << s;
    char answer;
    std::cin >> answer;
}
