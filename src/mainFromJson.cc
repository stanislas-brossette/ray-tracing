#include <iostream>
#include <chrono>
#include <vector>

#include "SceneLoader.hh"
#include "Renderer.hh"
#include "Window.hh"
#include "Scene.hh"
#include "Pixel.hh"
#include "utils.hh"

int main(int argc, char *argv[])
{
    std::string sceneJsonName("testScene");
    if(argc > 1)
        sceneJsonName = std::string(argv[1]);

    std::string path = std::string(DATA) + sceneJsonName + ".json";
    if(not fileExists(path))
    {
        std::cout << "Error: this file " << path << " does not exist" << std::endl;
        return 0;
    }

    SceneLoader sceneLoader(path);
    Scene scene(sceneLoader.sceneData_);
    Window window(scene.camera_.resX_, scene.camera_.resY_);
    Renderer renderer(sceneLoader.sceneData_.rData);
    renderer.renderParallel(scene, window, "This is the json scene");
    window.save("../images/" + sceneJsonName + ".bmp");
    return 0;
}
