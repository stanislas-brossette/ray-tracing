#include <iostream>
#include <chrono>
#include <vector>

#include "SceneLoader.hh"
#include "Renderer.hh"
#include "Window.hh"
#include "Scene.hh"
#include "Pixel.hh"

int main(void)
{
    std::string path("/home/stanislas/profiles/devel/src/raytracing/data/testScene.json");
    SceneLoader sceneLoader(path);
    std::cout << sceneLoader.sceneData_.describe();
    Scene scene(sceneLoader.sceneData_);
    Window window(scene.camera_.resX_, scene.camera_.resY_);
    Renderer renderer(sceneLoader.sceneData_.rData);
    renderer.renderParallel(scene, window, "This is the json scene");
    return 0;
}
