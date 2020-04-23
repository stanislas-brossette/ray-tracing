#include <iostream>
#include <chrono>
#include <vector>
#include <sstream>
#include <cstdlib>

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
    //std::cout << sceneLoader.sceneData_.describe() << std::endl;
    std::cout << scene << std::endl;
    bool display = sceneLoader.sceneData_.sExData.display;
    Window window(scene.camera_.resX_, scene.camera_.resY_, display);
    Renderer renderer(sceneLoader.sceneData_.rData);

    std::string imageFolderPath("../images/videoMeshMonkey/");
    for (size_t i = 0; i < 180; i++)
    {
        //scene.camera_.frame_.translateLocal(0.035, 0, 0);
        //scene.camera_.rotateToTarget();
        //scene.camera_.moveToDistanceToTarget(2.0);
        Vector3 rotAxis(0,0,1);
        scene.items_[2]->geometry_->f_.rotate(rotAxis, 2);
        scene.items_[1]->geometry_->f_.rotate(rotAxis, -0.1);
        window.clear();
        PerformanceTracker::instance().resetRays();
        renderer.renderParallel(scene, window, "");
        std::string imageFilePath(imageFolderPath + sceneJsonName + std::to_string(scene.camera_.resX_) + std::to_string(i) + ".bmp"); 
        window.save(imageFilePath);
        std::cout << PerformanceTracker::instance().describe();
    }
    SDL_Quit();

    std::string videoCreationCommand("ffmpeg -f image2 -r 30 -i " + imageFolderPath
            + sceneJsonName + std::to_string(scene.camera_.resX_) + "%d.bmp "
            + imageFolderPath + "out.mov");
    std::system(videoCreationCommand.c_str());
    std::cout << "Video saved in " << imageFolderPath << "out.mov" << std::endl;

    std::string cleanupCommand("rm " + imageFolderPath
            + sceneJsonName + std::to_string(scene.camera_.resX_) + "*.bmp");
    std::system(cleanupCommand.c_str());

    std::string playCommand("vlc " + imageFolderPath + "out.mov");
    std::system(playCommand.c_str());


    
    return 0;
}


