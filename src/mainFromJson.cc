#include <iostream>
#include <chrono>
#include <vector>

#include "SceneLoader.hh"
#include "Renderer.hh"
#include "Window.hh"
#include "Scene.hh"
#include "Pixel.hh"
#include "utils.hh"

int main(void)
{
    std::string sceneJsonName("tableScene.json");
    std::string path = std::string(DATA) + sceneJsonName;
    SceneLoader sceneLoader(path);
    Scene scene(sceneLoader.sceneData_);
    Window window(scene.camera_.resX_, scene.camera_.resY_);
    Renderer renderer(sceneLoader.sceneData_.rData);
    renderer.renderParallel(scene, window, "This is the json scene");
    return 0;
}
