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
    std::cout << sceneLoader.sceneData_.describe() << std::endl;
    Window window(scene.camera_.resX_, scene.camera_.resY_);
    Renderer renderer(sceneLoader.sceneData_.rData);
    window.save("../images/" + sceneJsonName + ".bmp");

    bool quit = false;
    SDL_Event event;

    bool rotationMode = false;
    double rotIncrement = 5;
    double transIncrement = 0.1;
 
    bool needRender = true;
    while (!quit)
    {
        if(needRender)
        {
            renderer.renderParallel(scene, window, "");
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
                    window.clear();
                    break;
                // Toggle simplified render with s
                case SDLK_s:
                    scene.toggleSimplifiedRender();
                    needRender = true;
                    break;
                // Toggle simplified render with s
                case SDLK_r:
                    rotationMode = not rotationMode;
                    break;
                // Move camera left right up down
                case SDLK_LEFT:
                    if(rotationMode)
                        scene.camera_.frame_.yaw(rotIncrement);
                    else
                        scene.translateCameraLocal(-transIncrement, 0.0, 0.0);
                    needRender = true;
                    break;
                case SDLK_RIGHT:
                    if(rotationMode)
                        scene.camera_.frame_.yaw(-rotIncrement);
                    else
                        scene.translateCameraLocal(transIncrement, 0.0, 0.0);
                    needRender = true;
                    break;
                case SDLK_UP:
                    if(rotationMode)
                        scene.camera_.frame_.pitch(rotIncrement);
                    else
                        scene.translateCameraLocal(0.0, transIncrement, 0.0);
                    needRender = true;
                    break;
                case SDLK_DOWN:
                    if(rotationMode)
                        scene.camera_.frame_.pitch(-rotIncrement);
                    else
                        scene.translateCameraLocal(0.0, -transIncrement, 0.0);
                    needRender = true;
                    break;
                case SDLK_PAGEUP:
                    if(rotationMode)
                        scene.camera_.frame_.roll(rotIncrement);
                    else
                        scene.translateCameraLocal(0.0, 0.0, transIncrement);
                    needRender = true;
                    break;
                case SDLK_PAGEDOWN:
                    if(rotationMode)
                        scene.camera_.frame_.roll(-rotIncrement);
                    else
                        scene.translateCameraLocal(0.0, 0.0, -transIncrement);
                    needRender = true;
                    break;
                // Change camera/windows resolution
                case SDLK_KP_MINUS:
                    scene.multiplyResolution(0.8);
                    window.changeResolution(scene.camera_.resX_, scene.camera_.resY_);
                    needRender = true;
                    break;
                case SDLK_KP_PLUS:
                    scene.multiplyResolution(1.2);
                    window.changeResolution(scene.camera_.resX_, scene.camera_.resY_);
                    needRender = true;
                    break;
            }
            break;
        }
    }
    SDL_Quit();
    
    return 0;
}

