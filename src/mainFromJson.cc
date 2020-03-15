#include <iostream>
#include <chrono>
#include <vector>

#include "SceneLoader.hh"
#include "Window.hh"
#include "Scene.hh"
#include "Pixel.hh"

int main(void)
{
    std::string path("/home/stanislas/profiles/devel/src/raytracing/data/testScene.json");
    SceneLoader sceneLoader;
    SceneData sData = sceneLoader.load(path);
    std::cout << sData.describe();
    Scene scene(sData);
    Window window(scene.camera_.resX_, scene.camera_.resY_);

    window.clear();
    int iter = 0;
    int nPixPerRender = sData.rData.nPixPerRender;
    std::chrono::steady_clock::time_point begin = std::chrono::steady_clock::now();
    while(iter < sData.rData.nLightRay)
    {
        std::vector<Pixel> pixs(nPixPerRender);
        scene.renderParallel(pixs, 0, nPixPerRender);
        window.addPixels(pixs);
        window.render();
        iter+=nPixPerRender;
    }
    std::chrono::steady_clock::time_point end = std::chrono::steady_clock::now();
    std::cout << "duration Parallel = " << std::chrono::duration_cast<std::chrono::microseconds>(end - begin).count() << "[µs]" << std::endl;;
    char answer;
    std::cin >> answer;
    return 0;
}
