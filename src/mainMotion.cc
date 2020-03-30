#include <iostream>
#include <chrono>
#include <vector>
#include <sstream>

#include <SDL.h>
#include <SDL_image.h>
#include <SDL_ttf.h>
#include <SDL_mixer.h>

#include "SceneLoader.hh"
#include "InputHandler.hh"
#include "Renderer.hh"
#include "Window.hh"
#include "Scene.hh"
#include "Pixel.hh"
#include "utils.hh"


int main(int argc, char *argv[])
{
    printManual();
    std::string sceneJsonName("testScene");
    if(argc > 1)
        sceneJsonName = std::string(argv[1]);

    std::string path = std::string(DATA) + sceneJsonName + ".json";
    if(not fileExists(path))
    {
        std::cout << "Error: this file " << path << " does not exist" << std::endl;
        return 0;
    }

    SDL_Init(SDL_INIT_VIDEO);

    SceneLoader sceneLoader(path);
    Scene scene(sceneLoader.sceneData_);
    std::cout << sceneLoader.sceneData_.describe() << std::endl;
    Window window(scene.camera_.resX_, scene.camera_.resY_);
    Renderer renderer(sceneLoader.sceneData_.rData);
    InputHandler inputHandler;
    window.save("../images/" + sceneJsonName + ".bmp");

    while (!inputHandler.quit)
    {
        if(inputHandler.needReload)
        {
            sceneLoader = SceneLoader(path);
            scene = Scene(sceneLoader.sceneData_);
            window.changeResolution(scene.camera_.resX_, scene.camera_.resY_);
            window.clear();
            inputHandler.needReload = false;
        }
        if(inputHandler.needRender)
        {
            renderer.renderParallel(scene, window, "");
            inputHandler.needRender = false;
        }
        inputHandler.handleInputs(window, scene);
    }
    SDL_Quit();
    
    return 0;
}

