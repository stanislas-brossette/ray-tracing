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
#include "MotionLoader.hh"
#include "InputHandler.hh"
#include "Renderer.hh"
#include "Window.hh"
#include "Scene.hh"
#include "Pixel.hh"
#include "utils.hh"


int main(int argc, char *argv[])
{
    printManual();
    std::string sceneJsonName("atalanteObj");
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
    //std::cout << scene << std::endl;
    bool display = sceneLoader.sceneData_.sExData.display;
    Window window(scene.camera_.resX_, scene.camera_.resY_, display);
    Renderer renderer(sceneLoader.sceneData_.rData);

    std::string imageFolderPath("/home/stanislas/wdc_workspace/src/ray-tracing/images/videoAtalante/");

    std::string motionJsonName("trajectories/patient_000052/foot_rolling_0_100");
    std::string motionPath = std::string(DATA) + motionJsonName + ".json";
    MotionLoader motionLoader(motionPath);

    for (int frameId = 0; frameId < 100; frameId++)
    {
        std::cout << "frameId: " << frameId << std::endl;
        scene.camera_.target_.x_ = frameId*2.1/283.0;
        scene.camera_.rotateToTarget();
        for (size_t i = 4; i < 17; i++)
        {
            scene.items_[i]->geometry_->f_.o_  = motionLoader.get(frameId, scene.items_[i]->name_, "pos");
            scene.items_[i]->geometry_->f_.vx_ = motionLoader.get(frameId, scene.items_[i]->name_, "vx");
            scene.items_[i]->geometry_->f_.vy_ = motionLoader.get(frameId, scene.items_[i]->name_, "vy");
            scene.items_[i]->geometry_->f_.vz_ = motionLoader.get(frameId, scene.items_[i]->name_, "vz");
        }


        window.clear();
        PerformanceTracker::instance().resetRays();
        renderer.renderParallel(scene, window, "");
        std::string imageFilePath(imageFolderPath + sceneJsonName + "_" + std::to_string(scene.camera_.resX_) + "_" + std::to_string(frameId) + ".bmp");
        window.save(imageFilePath);
        std::cout << PerformanceTracker::instance().describe();
    }

    motionJsonName = "trajectories/patient_000052/foot_rolling_100_200";
    motionPath = std::string(DATA) + motionJsonName + ".json";
    MotionLoader motionLoader1(motionPath);

    for (int frameId = 100; frameId < 200; frameId++)
    {
        std::cout << "frameId: " << frameId << std::endl;
        scene.camera_.target_.x_ = frameId*2.1/283.0;
        scene.camera_.rotateToTarget();
        for (size_t i = 4; i < 17; i++)
        {
            scene.items_[i]->geometry_->f_.o_  = motionLoader1.get(frameId, scene.items_[i]->name_, "pos");
            scene.items_[i]->geometry_->f_.vx_ = motionLoader1.get(frameId, scene.items_[i]->name_, "vx");
            scene.items_[i]->geometry_->f_.vy_ = motionLoader1.get(frameId, scene.items_[i]->name_, "vy");
            scene.items_[i]->geometry_->f_.vz_ = motionLoader1.get(frameId, scene.items_[i]->name_, "vz");
        }


        window.clear();
        PerformanceTracker::instance().resetRays();
        renderer.renderParallel(scene, window, "");
        std::string imageFilePath(imageFolderPath + sceneJsonName + "_" + std::to_string(scene.camera_.resX_) + "_" + std::to_string(frameId) + ".bmp");
        window.save(imageFilePath);
        std::cout << PerformanceTracker::instance().describe();
    }

    motionJsonName = "trajectories/patient_000052/foot_rolling_200_300";
    motionPath = std::string(DATA) + motionJsonName + ".json";
    MotionLoader motionLoader2(motionPath);

    for (int frameId = 200; frameId < 284; frameId++)
    {
        std::cout << "frameId: " << frameId << std::endl;
        scene.camera_.target_.x_ = frameId*2.1/283.0;
        scene.camera_.rotateToTarget();
        for (size_t i = 4; i < 17; i++)
        {
            scene.items_[i]->geometry_->f_.o_  = motionLoader2.get(frameId, scene.items_[i]->name_, "pos");
            scene.items_[i]->geometry_->f_.vx_ = motionLoader2.get(frameId, scene.items_[i]->name_, "vx");
            scene.items_[i]->geometry_->f_.vy_ = motionLoader2.get(frameId, scene.items_[i]->name_, "vy");
            scene.items_[i]->geometry_->f_.vz_ = motionLoader2.get(frameId, scene.items_[i]->name_, "vz");
        }


        window.clear();
        PerformanceTracker::instance().resetRays();
        renderer.renderParallel(scene, window, "");
        std::string imageFilePath(imageFolderPath + sceneJsonName + "_" + std::to_string(scene.camera_.resX_) + "_" + std::to_string(frameId) + ".bmp");
        window.save(imageFilePath);
        std::cout << PerformanceTracker::instance().describe();
    }
    std::cout << "out of render" << std::endl;
    SDL_Quit();

    std::string imageFileBasePath(imageFolderPath + sceneJsonName + "_" + std::to_string(scene.camera_.resX_) + "_");
    std::string videoName(sceneJsonName + "_" + std::to_string(scene.camera_.resX_) + ".mkv");

    std::string videoCreationCommand("ffmpeg -r 30 -i " + imageFileBasePath + "%d.bmp " + videoName);
    std::cout << "videoCreationCommand.c_str(): " << videoCreationCommand.c_str() << std::endl;
    std::system(videoCreationCommand.c_str());
    std::cout << "Video saved in " << videoName << std::endl;

    //std::string cleanupCommand("rm " + imageFolderPath
    //        + sceneJsonName + std::to_string(scene.camera_.resX_) + "*.bmp");
    //std::system(cleanupCommand.c_str());

    //std::string playCommand("vlc " + videoName);
    //std::system(playCommand.c_str());


    
    return 0;
}


