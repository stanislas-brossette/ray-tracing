#include <iostream>
#include <chrono>
#include <vector>
#include <sstream>

#include <SDL.h>
#include <SDL_image.h>
#include <SDL_ttf.h>
#include <SDL_mixer.h>

#include "SceneLoader.hh"
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
    Window myWindow(scene.camera_.resX_, scene.camera_.resY_);
    Renderer renderer(sceneLoader.sceneData_.rData);

    bool quit = false;
    SDL_Event event;
 
    bool needRender = true;
    while (!quit)
    {
        if(needRender)
        {
            renderer.renderParallel(scene, myWindow, "");
            needRender = false;
        }
        SDL_WaitEvent(&event);
        SDL_Delay(5);
 
        switch (event.type)
        {
        case SDL_QUIT:
            quit = true;
            break;
        case SDL_KEYDOWN:
            switch (event.key.keysym.sym)
            {
                // Quit with q
                case SDLK_q:
                    quit = true;
                    break;
                // Refresh with F5
                case SDLK_F5:
                    needRender = true;
                    break;
                // Clear with c
                case SDLK_c:
                    myWindow.clear();
                    break;
                // Toggle simplified render with s
                case SDLK_s:
                    scene.toggleSimplifiedRender();
                    needRender = true;
                    break;
                // Move camera left right up down
                case SDLK_LEFT:
                    scene.translateCamera(-0.1, 0.0, 0.0);
                    needRender = true;
                    break;
                case SDLK_RIGHT:
                    scene.translateCamera(0.1, 0.0, 0.0);
                    needRender = true;
                    break;
                case SDLK_UP:
                    scene.translateCamera(0.0, 0.1, 0.0);
                    needRender = true;
                    break;
                case SDLK_DOWN:
                    scene.translateCamera(0.0, -0.1, 0.0);
                    needRender = true;
                    break;
                case SDLK_PAGEUP:
                    scene.translateCamera(0.0, 0.0, 0.1);
                    needRender = true;
                    break;
                case SDLK_PAGEDOWN:
                    scene.translateCamera(0.0, 0.0, -0.1);
                    needRender = true;
                    break;
                case SDLK_KP_MINUS:
                    scene.multiplyResolution(0.8);
                    myWindow.changeResolution(scene.camera_.resX_, scene.camera_.resY_);
                    needRender = true;
                    break;
                case SDLK_KP_PLUS:
                    scene.multiplyResolution(1.2);
                    myWindow.changeResolution(scene.camera_.resX_, scene.camera_.resY_);
                    needRender = true;
                    break;
            }
            break;
        }
    }
    SDL_Quit();
    
    return 0;
}

