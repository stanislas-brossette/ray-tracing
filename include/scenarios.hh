#pragma once

#include <iostream>
#include <chrono>
#include <vector>
#include <sstream>
#include <cstdlib>
#include <cmath>

#include <SDL.h>
#include <SDL_image.h>
#include <SDL_ttf.h>
#include <SDL_mixer.h>

#include "StringException.hh"
#include "SceneLoader.hh"
#include "MotionLoader.hh"
#include "InputHandler.hh"
#include "Renderer.hh"
#include "Window.hh"
#include "Scene.hh"
#include "Pixel.hh"
#include "utils.hh"

void transition()
{
    std::string sceneJsonName("atalanteStlDisco");
  
    std::string path = std::string(DATA) + sceneJsonName + ".json";
    if(not fileExists(path))
    {
        StringException error("Error: this file " + path + " does not exist");
        throw error;
    }
  
    SDL_Init(SDL_INIT_VIDEO);
  
    SceneLoader sceneLoader(path);
    Scene scene(sceneLoader.sceneData_);
    bool display = sceneLoader.sceneData_.sExData.display;
    Window window(scene.camera_.resX_, scene.camera_.resY_, display);
    Renderer renderer(sceneLoader.sceneData_.rData);
  
    std::string imageFolderPath(std::string(DATA) + "../images/videoAtalante/individualImages/");
  
    /******************
    *  Foot Rolling  *
    ******************/
    std::vector<std::string> motionJsonNames{
        "trajectories/patient_000052/foot_rolling_0_100",
        "trajectories/patient_000052/foot_rolling_100_200",
        "trajectories/patient_000052/foot_rolling_200_300"
    };
    std::vector<int> frameStartFile{0, 100, 200};
    std::vector<int> frameEndFile{100, 200, 284};
  
    std::string motionPath = std::string(DATA) + motionJsonNames[0] + ".json";
    MotionLoader motionLoader(motionPath);
    for (size_t i = 4; i < 17; i++)
    {
        scene.items_[i]->geometry_->f_.o_  = motionLoader.get(0, scene.items_[i]->name_, "pos");
        scene.items_[i]->geometry_->f_.vx_ = motionLoader.get(0, scene.items_[i]->name_, "vx");
        scene.items_[i]->geometry_->f_.vy_ = motionLoader.get(0, scene.items_[i]->name_, "vy");
        scene.items_[i]->geometry_->f_.vz_ = motionLoader.get(0, scene.items_[i]->name_, "vz");
    }

    std::string imageFileBasePath(imageFolderPath + "transition_" + std::to_string(scene.camera_.resX_) + "_");
  
    int nFrames = 720;
    double deltaAngle = 2*3.14159/nFrames;
    double percentFramesLightChange = 0.15; //percent of frame to go to or from dark
    double percentFramesColorChange = 0.50; //total percent to change color
    for (int frameId = 0; frameId < nFrames; frameId++)
    {
        //Disco switch
        if (frameId%50 == 5)
            scene.items_[3]->material_->texture_->switchColors();
        std::cout << "frameId: " << frameId << std::endl;
  
        //Rotate camera
        scene.camera_.frame_.o_ = Vector3(0.04473088918756667, -0.023698960694729244, 0.9304836521315684)
            + Vector3(1.5*std::cos(deltaAngle*frameId), 1.5*std::sin(deltaAngle*frameId), 0.2);
        scene.camera_.rotateToTarget();
  
        //Change robot color
        int nFramesColorChange = nFrames*percentFramesColorChange;
        int frameStartChangeColor = (0.5-percentFramesColorChange/2)*nFrames;
        int frameEndChangeColor = (0.5+percentFramesColorChange/2)*nFrames;
  
        //Initialize robot colors and floor
        if(frameId == 0)
        {
            Vector3RGB c(20.0/255.0);
            double reflec = 0.01;
            scene.items_[3]->geometry_->f_.o_.z_ = -1.0;
            scene.items_[17]->geometry_->f_.o_.z_ = 0.0;
            for (size_t i = 4; i < 17; i++)
            {
                scene.items_[i]->material_->texture_->setColor(c);
                scene.items_[i]->material_->reflectiveness_ = reflec;
            }
            scene.ambiantLight_.backGroundColor_ = Vector3RGB(1.0, 1.0, 1.0);
        }
        if(frameId > frameStartChangeColor and frameId < frameEndChangeColor)
        {
            Vector3RGB c((20.0 + (230.0-20.0)*(frameId-frameStartChangeColor)/nFramesColorChange)/255.0);
            double reflec = 0.01 + (0.5-0.01)*(frameId-frameStartChangeColor)/nFramesColorChange;
            for (size_t i = 4; i < 17; i++)
            {
                scene.items_[i]->material_->texture_->setColor(c);
                scene.items_[i]->material_->reflectiveness_ = reflec;
            }
        }
  
        if(frameId == 0)
        {
            scene.items_[0]->material_->texture_->setColor(Vector3RGB(240.0/255.0, 240.0/255.0, 240.0/255.0));
            scene.items_[1]->material_->texture_->setColor(Vector3RGB(240.0/255.0, 240.0/255.0, 240.0/255.0));
            scene.items_[2]->material_->texture_->setColor(Vector3RGB(240.0/255.0, 240.0/255.0, 240.0/255.0));
        }
        //Change lights
        int nFramesLightChange = nFrames*percentFramesLightChange;
        if(frameId > (0.5-percentFramesLightChange)*nFrames and frameId < (0.5+percentFramesLightChange)*nFrames)
        {
            if(frameId < nFrames/2)
            {
                scene.items_[0]->material_->lightIntensity_ -= 0.8*sceneLoader.sceneData_.itemsData[0].mData->lightIntensity/nFramesLightChange;
                scene.items_[1]->material_->lightIntensity_ -= 0.8*sceneLoader.sceneData_.itemsData[1].mData->lightIntensity/nFramesLightChange;
                scene.items_[2]->material_->lightIntensity_ -= 0.8*sceneLoader.sceneData_.itemsData[2].mData->lightIntensity/nFramesLightChange;
                scene.ambiantLight_.intensity_ -= 0.8*sceneLoader.sceneData_.aData.intensity/nFramesLightChange;
                scene.ambiantLight_.backGroundColor_.r_ -= 1.0/nFramesLightChange;
                scene.ambiantLight_.backGroundColor_.g_ -= 1.0/nFramesLightChange;
                scene.ambiantLight_.backGroundColor_.b_ -= 1.0/nFramesLightChange;
                scene.ambiantLight_.backGroundColor_.clamp();
            }
            else
            {
                scene.items_[0]->material_->lightIntensity_ += 0.8*sceneLoader.sceneData_.itemsData[0].mData->lightIntensity/nFramesLightChange;
                scene.items_[1]->material_->lightIntensity_ += 0.8*sceneLoader.sceneData_.itemsData[1].mData->lightIntensity/nFramesLightChange;
                scene.items_[2]->material_->lightIntensity_ += 0.8*sceneLoader.sceneData_.itemsData[2].mData->lightIntensity/nFramesLightChange;
                scene.ambiantLight_.intensity_ += 0.8*sceneLoader.sceneData_.aData.intensity/nFramesLightChange;
            }
            if(frameId == nFrames/2)
            {
                scene.items_[0]->material_->texture_->setColor(Vector3RGB(15.0/255.0, 192.0/255.0, 252.0/255.0));
                scene.items_[1]->material_->texture_->setColor(Vector3RGB(255.0/255.0, 29.0/255.0, 175.0/255.0));
                scene.items_[2]->material_->texture_->setColor(Vector3RGB(212.0/255.0, 255.0/255.0, 71.0/255));
            }
        }
  
  
        //Exchange checker and disco floor
        if(frameId == nFrames/2)
        {
            scene.items_[3]->geometry_->f_.o_.z_ = 0.0;
            scene.items_[17]->geometry_->f_.o_.z_ = -1.0;
        }
  
        window.clear();
        PerformanceTracker::instance().resetRays();
        renderer.renderParallel(scene, window, "");
        std::string imageFilePath(imageFileBasePath + std::to_string(frameId) + ".bmp");
        window.save(imageFilePath);
        std::cout << PerformanceTracker::instance().describe();
    }
    std::cout << "out of render" << std::endl;
    SDL_Quit();
  
    std::string videoName("scenarios/transition_" + std::to_string(scene.camera_.resX_) + "_60fps.mkv");
  
    std::string videoCreationCommand("ffmpeg -r 60 -i " + imageFileBasePath + "%d.bmp " + videoName);
    std::cout << "videoCreationCommand.c_str(): " << videoCreationCommand.c_str() << std::endl;
    std::system(videoCreationCommand.c_str());
    std::cout << "Video saved in " << videoName << std::endl;
    return;
}

void footRollingTraveling()
{
    std::string sceneJsonName("atalanteStlDisco");

    std::string path = std::string(DATA) + sceneJsonName + ".json";
    if(not fileExists(path))
    {
        StringException error("Error: this file " + path + " does not exist");
        throw error;
    }

    SDL_Init(SDL_INIT_VIDEO);

    SceneLoader sceneLoader(path);
    Scene scene(sceneLoader.sceneData_);
    bool display = sceneLoader.sceneData_.sExData.display;
    Window window(scene.camera_.resX_, scene.camera_.resY_, display);
    Renderer renderer(sceneLoader.sceneData_.rData);

    std::string imageFolderPath(std::string(DATA) + "../images/videoAtalante/individualImages/");
    std::string imageFileBasePath(imageFolderPath + "footRollingTraveling_" + std::to_string(scene.camera_.resX_) + "_");

    /**********************
    *  Foot Rolling 60fps *
    **********************/
    std::vector<std::string> motionJsonNames{
        "trajectories/patient_000052/foot_rolling_60fps_0_100",
        "trajectories/patient_000052/foot_rolling_60fps_100_200",
        "trajectories/patient_000052/foot_rolling_60fps_200_300",
        "trajectories/patient_000052/foot_rolling_60fps_300_400",
        "trajectories/patient_000052/foot_rolling_60fps_400_500"
    };
    std::vector<int> frameStartFile{0, 100, 200, 300, 400};
    std::vector<int> frameEndFile{100, 200, 300, 400, 407};

    for (size_t motionFileIndex = 0; motionFileIndex < motionJsonNames.size(); motionFileIndex++)
    {
        std::string motionPath = std::string(DATA) + motionJsonNames[motionFileIndex] + ".json";
        MotionLoader motionLoader(motionPath);
        for (int frameId = frameStartFile[motionFileIndex]; frameId < frameEndFile[motionFileIndex]; frameId++)
        {
            if(frameId == 0)
            {
                scene.ambiantLight_.backGroundColor_ = Vector3RGB(0.9, 0.9, 0.9);
                Vector3RGB c(20.0/255.0);
                double reflec = 0.01;
                scene.items_[3]->geometry_->f_.o_.z_ = -1.0;
                scene.items_[17]->geometry_->f_.o_.z_ = 0.0;
                for (size_t i = 4; i < 17; i++)
                {
                    scene.items_[i]->material_->texture_->setColor(c);
                    scene.items_[i]->material_->reflectiveness_ = reflec;
                }
                scene.camera_.frame_.o_ = Vector3(0.3,-1.25,0.85);
                scene.camera_.target_ = Vector3(0.2, 0, 0.75);
                scene.camera_.rotateToTarget();
                scene.items_[0]->material_->texture_->setColor(Vector3RGB(1));
                scene.items_[0]->geometry_->f_.o_ = Vector3(0, -0.7, 1.8);
                scene.items_[1]->material_->texture_->setColor(Vector3RGB(1));
                scene.items_[1]->geometry_->f_.o_ = Vector3(1.2, -0.7, 1.8);
                scene.items_[2]->material_->texture_->setColor(Vector3RGB(1));
                scene.items_[2]->geometry_->f_.o_ = Vector3(2.4, -0.7, 1.8);
            }
            std::cout << "frameId: " << frameId << std::endl;
            //scene.camera_.target_.x_ = frameId*2.1/283.0;
            scene.camera_.frame_.o_.x_ += 2.4/frameEndFile.back();
            //scene.camera_.frame_.o_.z_ -= 1.4/283.0;
            //scene.camera_.target_.z_ -= 1.4/283.0;
            //scene.camera_.rotateToTarget();
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
            std::string imageFilePath(imageFileBasePath + std::to_string(frameId) + ".bmp");
            window.save(imageFilePath);
            std::cout << PerformanceTracker::instance().describe();
        }
    }
    std::cout << "out of render" << std::endl;
    SDL_Quit();


    std::string videoName("scenarios/footRollingTraveling_" + std::to_string(scene.camera_.resX_) + ".mkv");

    std::string videoCreationCommand("ffmpeg -r 60 -i " + imageFileBasePath + "%d.bmp " + videoName);
    std::cout << "videoCreationCommand.c_str(): " << videoCreationCommand.c_str() << std::endl;
    std::system(videoCreationCommand.c_str());
    std::cout << "Video saved in " << videoName << std::endl;

    return;
}

void footRollingPan()
{
    std::string sceneJsonName("atalanteStlDisco");

    std::string path = std::string(DATA) + sceneJsonName + ".json";
    if(not fileExists(path))
    {
        StringException error("Error: this file " + path + " does not exist");
        throw error;
    }

    SDL_Init(SDL_INIT_VIDEO);

    SceneLoader sceneLoader(path);
    Scene scene(sceneLoader.sceneData_);
    bool display = sceneLoader.sceneData_.sExData.display;
    Window window(scene.camera_.resX_, scene.camera_.resY_, display);
    Renderer renderer(sceneLoader.sceneData_.rData);

    std::string imageFolderPath(std::string(DATA) + "../images/videoAtalante/individualImages/");
    std::string imageFileBasePath(imageFolderPath + "footRollingPan_" + std::to_string(scene.camera_.resX_) + "_");

    /**********************
    *  Foot Rolling 60fps *
    **********************/
    std::vector<std::string> motionJsonNames{
        "trajectories/patient_000052/foot_rolling_60fps_0_100",
        "trajectories/patient_000052/foot_rolling_60fps_100_200",
        "trajectories/patient_000052/foot_rolling_60fps_200_300",
        "trajectories/patient_000052/foot_rolling_60fps_300_400",
        "trajectories/patient_000052/foot_rolling_60fps_400_500"
    };
    std::vector<int> frameStartFile{0, 100, 200, 300, 400};
    std::vector<int> frameEndFile{100, 200, 300, 400, 407};

    for (size_t motionFileIndex = 0; motionFileIndex < motionJsonNames.size(); motionFileIndex++)
    {
        std::string motionPath = std::string(DATA) + motionJsonNames[motionFileIndex] + ".json";
        MotionLoader motionLoader(motionPath);
        for (int frameId = frameStartFile[motionFileIndex]; frameId < frameEndFile[motionFileIndex]; frameId++)
        {
            if(frameId == 0)
            {
                scene.ambiantLight_.backGroundColor_ = Vector3RGB(0.9, 0.9, 0.9);
                Vector3RGB c(20.0/255.0);
                double reflec = 0.01;
                scene.items_[3]->geometry_->f_.o_.z_ = -1.0;
                scene.items_[17]->geometry_->f_.o_.z_ = 0.0;
                for (size_t i = 4; i < 17; i++)
                {
                    scene.items_[i]->material_->texture_->setColor(c);
                    scene.items_[i]->material_->reflectiveness_ = reflec;
                }
                scene.camera_.frame_.o_ = Vector3(-0.3, -0.50, 1.4);
                scene.camera_.target_ = Vector3(-0.3, 0, 1.4);
                scene.camera_.rotateToTarget();
                scene.items_[0]->material_->texture_->setColor(Vector3RGB(1));
                scene.items_[0]->geometry_->f_.o_ = Vector3(0, -0.7, 1.8);
                scene.items_[1]->material_->texture_->setColor(Vector3RGB(1));
                scene.items_[1]->geometry_->f_.o_ = Vector3(1.2, -0.7, 1.8);
                scene.items_[2]->material_->texture_->setColor(Vector3RGB(1));
                scene.items_[2]->geometry_->f_.o_ = Vector3(2.4, -0.7, 1.8);
            }
            std::cout << "frameId: " << frameId << std::endl;
            scene.camera_.frame_.o_.x_ += 2.5/frameEndFile.back();
            scene.camera_.frame_.o_.z_ -= 1.35/frameEndFile.back();
            scene.camera_.target_.x_ += 2.5/frameEndFile.back();
            scene.camera_.target_.z_ -= 1.35/frameEndFile.back();
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
            std::string imageFilePath(imageFileBasePath + std::to_string(frameId) + ".bmp");
            window.save(imageFilePath);
            std::cout << PerformanceTracker::instance().describe();
        }
    }
    std::cout << "out of render" << std::endl;
    SDL_Quit();


    std::string videoName("scenarios/footRollingPan_" + std::to_string(scene.camera_.resX_) + ".mkv");

    std::string videoCreationCommand("ffmpeg -r 60 -i " + imageFileBasePath + "%d.bmp " + videoName);
    std::cout << "videoCreationCommand.c_str(): " << videoCreationCommand.c_str() << std::endl;
    std::system(videoCreationCommand.c_str());
    std::cout << "Video saved in " << videoName << std::endl;

    return;
}


void flatFootDisco()
{
    std::string sceneJsonName("atalanteStlDisco");

    std::string path = std::string(DATA) + sceneJsonName + ".json";
    if(not fileExists(path))
    {
        StringException error("Error: this file " + path + " does not exist");
        throw error;
    }

    SDL_Init(SDL_INIT_VIDEO);

    SceneLoader sceneLoader(path);
    Scene scene(sceneLoader.sceneData_);
    bool display = sceneLoader.sceneData_.sExData.display;
    Window window(scene.camera_.resX_, scene.camera_.resY_, display);
    Renderer renderer(sceneLoader.sceneData_.rData);

    std::string imageFolderPath(std::string(DATA) + "../images/videoAtalante/individualImages/");
    std::string imageFileBasePath(imageFolderPath + "flatFootDisco_" + std::to_string(scene.camera_.resX_) + "_");

    /***************
    *  Flat Foot  *
    ***************/
    std::vector<std::string> motionJsonNames{
        "trajectories/patient_000052/flat_foot_60fps_0_100",
        "trajectories/patient_000052/flat_foot_60fps_100_200",
        "trajectories/patient_000052/flat_foot_60fps_200_300"
    };
    std::vector<int> frameStartFile{0, 100, 200};
    std::vector<int> frameEndFile{100, 200, 299};

    for (size_t motionFileIndex = 0; motionFileIndex < motionJsonNames.size(); motionFileIndex++)
    {
        std::string motionPath = std::string(DATA) + motionJsonNames[motionFileIndex] + ".json";
        MotionLoader motionLoader(motionPath);
        for (int frameId = frameStartFile[motionFileIndex]; frameId < frameEndFile[motionFileIndex]; frameId++)
        {
            //Disco switch
            if (frameId%50 == 5)
                scene.items_[3]->material_->texture_->switchColors();
            if(frameId == 0)
            {
                //Vector3RGB c(20.0/255.0);
                //double reflec = 0.01;
                //scene.items_[3]->geometry_->f_.o_.z_ = -1.0;
                //scene.items_[17]->geometry_->f_.o_.z_ = 0.0;
                //for (size_t i = 4; i < 17; i++)
                //{
                //    scene.items_[i]->material_->texture_->setColor(c);
                //    scene.items_[i]->material_->reflectiveness_ = reflec;
                //}
                scene.camera_.frame_.o_ = Vector3(2.0, 0.0, 2.0);
                scene.camera_.target_ = Vector3(0.2, 0, 0.5);
                scene.camera_.rotateToTarget();

                scene.items_[0]->material_->texture_->setColor(Vector3RGB(1));
                scene.items_[0]->material_->lightIntensity_ = 1.0;
                scene.items_[0]->geometry_->f_.o_ = Vector3(-0.5, 0.0, 1.8);

                scene.items_[1]->material_->texture_->setColor(Vector3RGB(1));
                scene.items_[1]->material_->lightIntensity_ = 1.0;
                scene.items_[1]->geometry_->f_.o_ = Vector3(1.2, -0.7, 1.8);

                scene.items_[2]->material_->texture_->setColor(Vector3RGB(1));
                scene.items_[2]->material_->lightIntensity_ = 1.0;
                scene.items_[2]->geometry_->f_.o_ = Vector3(1.2, 0.7, 1.8);
            }
            else if (frameId == 60)
            {
                scene.items_[0]->material_->lightIntensity_ = 1.0;
                scene.items_[1]->material_->lightIntensity_ = 0.0;
                scene.items_[2]->material_->lightIntensity_ = 0.0;
                scene.items_[0]->geometry_->f_.o_ = Vector3(-1.0, 0.0, 1.8);
                scene.items_[1]->geometry_->f_.o_ = Vector3(100.7, -0.7, 1.8);
                scene.items_[2]->geometry_->f_.o_ = Vector3(100.7, 0.7, 1.8);
            }
            else if (frameId == 120)
            {
                scene.items_[0]->material_->lightIntensity_ = 0.0;
                scene.items_[1]->material_->lightIntensity_ = 1.0;
                scene.items_[2]->material_->lightIntensity_ = 0.0;
                scene.items_[0]->geometry_->f_.o_ = Vector3(100.0, 0.0, 1.8);
                scene.items_[1]->geometry_->f_.o_ = Vector3(0.7, -0.7, 1.8);
                scene.items_[2]->geometry_->f_.o_ = Vector3(100.7, 0.7, 1.8);
            }
            else if (frameId == 180)
            {
                scene.items_[0]->material_->lightIntensity_ = 0.0;
                scene.items_[1]->material_->lightIntensity_ = 0.0;
                scene.items_[2]->material_->lightIntensity_ = 1.0;
                scene.items_[0]->geometry_->f_.o_ = Vector3(100.0, 0.0, 1.8);
                scene.items_[1]->geometry_->f_.o_ = Vector3(100.7, -0.7, 1.8);
                scene.items_[2]->geometry_->f_.o_ = Vector3(0.7, 0.7, 1.8);
            }
            else if (frameId == 240)
            {
                scene.items_[0]->material_->lightIntensity_ = 1.0;
                scene.items_[1]->material_->lightIntensity_ = 1.0;
                scene.items_[2]->material_->lightIntensity_ = 1.0;
                scene.items_[0]->geometry_->f_.o_ = Vector3(-1.0, 0.0, 1.8);
                scene.items_[1]->geometry_->f_.o_ = Vector3(0.7, -0.7, 1.8);
                scene.items_[2]->geometry_->f_.o_ = Vector3(0.7, 0.7, 1.8);
            }


            std::cout << "frameId: " << frameId << std::endl;
            //scene.camera_.target_.x_ = frameId*2.1/283.0;
            //scene.camera_.frame_.o_.x_ += 2.4/frameEndFile.back();
            //scene.camera_.frame_.o_.z_ -= 1.4/283.0;
            //scene.camera_.target_.z_ -= 1.4/283.0;
            //scene.camera_.rotateToTarget();
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
            std::string imageFilePath(imageFileBasePath + std::to_string(frameId) + ".bmp");
            window.save(imageFilePath);
            std::cout << PerformanceTracker::instance().describe();
        }
    }
    std::cout << "out of render" << std::endl;
    SDL_Quit();


    std::string videoName("scenarios/flatFootDisco_" + std::to_string(scene.camera_.resX_) + ".mkv");

    std::string videoCreationCommand("ffmpeg -r 60 -i " + imageFileBasePath + "%d.bmp " + videoName);
    std::cout << "videoCreationCommand.c_str(): " << videoCreationCommand.c_str() << std::endl;
    std::system(videoCreationCommand.c_str());
    std::cout << "Video saved in " << videoName << std::endl;

    return;
}

void footRollingDisco()
{
    std::string sceneJsonName("atalanteStlDisco");

    std::string path = std::string(DATA) + sceneJsonName + ".json";
    if(not fileExists(path))
    {
        StringException error("Error: this file " + path + " does not exist");
        throw error;
    }

    SDL_Init(SDL_INIT_VIDEO);

    SceneLoader sceneLoader(path);
    Scene scene(sceneLoader.sceneData_);
    bool display = sceneLoader.sceneData_.sExData.display;
    Window window(scene.camera_.resX_, scene.camera_.resY_, display);
    Renderer renderer(sceneLoader.sceneData_.rData);

    std::string imageFolderPath(std::string(DATA) + "../images/videoAtalante/individualImages/");
    std::string imageFileBasePath(imageFolderPath + "footRollingDisco_" + std::to_string(scene.camera_.resX_) + "_");

    /***************
    *  Foot Rolling  *
    ***************/
    std::vector<std::string> motionJsonNames{
        "trajectories/patient_000052/foot_rolling_60fps_0_100",
        "trajectories/patient_000052/foot_rolling_60fps_100_200",
        "trajectories/patient_000052/foot_rolling_60fps_200_300",
        "trajectories/patient_000052/foot_rolling_60fps_300_400",
        "trajectories/patient_000052/foot_rolling_60fps_400_500"
    };
    std::vector<int> frameStartFile{0, 100, 200, 300, 400};
    std::vector<int> frameEndFile{100, 200, 300, 400, 407};

    for (size_t motionFileIndex = 0; motionFileIndex < motionJsonNames.size(); motionFileIndex++)
    {
        std::string motionPath = std::string(DATA) + motionJsonNames[motionFileIndex] + ".json";
        MotionLoader motionLoader(motionPath);
        for (int frameId = frameStartFile[motionFileIndex]; frameId < frameEndFile[motionFileIndex]; frameId++)
        {
            //Disco switch
            if (frameId%50 == 25)
                scene.items_[3]->material_->texture_->switchColors();
            if(frameId == 0)
            {
                scene.camera_.frame_.o_ = Vector3(2.65564,1.56392,1.26467);
                scene.camera_.target_ = Vector3(0.7, 0, 1.0);
                scene.camera_.rotateToTarget();

                scene.ambiantLight_.intensity_ = 0.0;

                //scene.items_[0]->material_->texture_->setColor(Vector3RGB(1));
                scene.items_[0]->material_->lightIntensity_ = 1.0;
                scene.items_[0]->geometry_->f_.o_ = Vector3(0.0, 0.0, 1.7);

                //scene.items_[1]->material_->texture_->setColor(Vector3RGB(1));
                scene.items_[1]->material_->lightIntensity_ = 1.0;
                scene.items_[1]->geometry_->f_.o_ = Vector3(1.2, 0.0, 1.7);

                //scene.items_[2]->material_->texture_->setColor(Vector3RGB(1));
                scene.items_[2]->material_->lightIntensity_ = 30.0;
                scene.items_[2]->geometry_->f_.o_ = Vector3(5.0, 5.0, 7.0);

                //Reposition mirrors
                scene.items_[18]->material_->reflectiveness_ = 0.9;
                scene.items_[18]->geometry_->f_.o_ = Vector3(-1.02, 0.0, 0.0);
                scene.items_[18]->geometry_->f_.vx_ = Vector3(0.0, 0.0, -1.0);
                scene.items_[18]->geometry_->f_.vy_ = Vector3(0.0, 1.0, 0.0);
                scene.items_[18]->geometry_->f_.vz_ = Vector3(1.0, 0.0, 0.0);

                scene.items_[19]->material_->reflectiveness_ = 0.9;
                scene.items_[19]->geometry_->f_.o_ = Vector3(0.0, -1.02, 0.0);
                scene.items_[19]->geometry_->f_.vx_ = Vector3(1.0, 0.0, 0.0);
                scene.items_[19]->geometry_->f_.vy_ = Vector3(0.0, 0.0, -1.0);
                scene.items_[19]->geometry_->f_.vz_ = Vector3(0.0, 1.0, 0.0);
            }

            std::cout << "frameId: " << frameId << std::endl;
            //scene.camera_.target_.x_ = frameId*2.1/283.0;
            //scene.camera_.frame_.o_.x_ += 2.4/frameEndFile.back();
            //scene.camera_.frame_.o_.z_ -= 1.4/283.0;
            //scene.camera_.target_.z_ -= 1.4/283.0;
            //scene.camera_.rotateToTarget();
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
            std::string imageFilePath(imageFileBasePath + std::to_string(frameId) + ".bmp");
            window.save(imageFilePath);
            std::cout << PerformanceTracker::instance().describe();
        }
    }
    std::cout << "out of render" << std::endl;
    SDL_Quit();


    std::string videoName("scenarios/footRollingDisco_" + std::to_string(scene.camera_.resX_) + ".mkv");

    std::string videoCreationCommand("ffmpeg -r 60 -i " + imageFileBasePath + "%d.bmp " + videoName);
    std::cout << "videoCreationCommand.c_str(): " << videoCreationCommand.c_str() << std::endl;
    std::system(videoCreationCommand.c_str());
    std::cout << "Video saved in " << videoName << std::endl;

    return;
}

void turnaroundDisco()
{
    std::string sceneJsonName("atalanteStlDisco");

    std::string path = std::string(DATA) + sceneJsonName + ".json";
    if(not fileExists(path))
    {
        StringException error("Error: this file " + path + " does not exist");
        throw error;
    }

    SDL_Init(SDL_INIT_VIDEO);

    SceneLoader sceneLoader(path);
    Scene scene(sceneLoader.sceneData_);
    bool display = sceneLoader.sceneData_.sExData.display;
    Window window(scene.camera_.resX_, scene.camera_.resY_, display);
    Renderer renderer(sceneLoader.sceneData_.rData);

    std::string imageFolderPath(std::string(DATA) + "../images/videoAtalante/individualImages/");
    std::string imageFileBasePath(imageFolderPath + "turnaroundDisco_" + std::to_string(scene.camera_.resX_) + "_");

    /***************
    *  turnaround  *
    ***************/
    std::vector<std::string> motionJsonNames{
        "trajectories/patient_000052/turnaround_60fps_0_100",
        "trajectories/patient_000052/turnaround_60fps_100_200",
        "trajectories/patient_000052/turnaround_60fps_200_300",
        "trajectories/patient_000052/turnaround_60fps_300_400",
        "trajectories/patient_000052/turnaround_60fps_400_500",
        "trajectories/patient_000052/turnaround_60fps_500_600"
    };
    std::vector<int> frameStartFile{0, 100, 200, 300, 400, 500};
    std::vector<int> frameEndFile{100, 200, 300, 400, 500, 599};

    for (size_t motionFileIndex = 0; motionFileIndex < motionJsonNames.size(); motionFileIndex++)
    {
        std::string motionPath = std::string(DATA) + motionJsonNames[motionFileIndex] + ".json";
        MotionLoader motionLoader(motionPath);
        for (int frameId = frameStartFile[motionFileIndex]; frameId < frameEndFile[motionFileIndex]; frameId++)
        {
            //Disco switch
            if (frameId%50 == 17)
                scene.items_[3]->material_->texture_->switchColors();
            if(frameId == 0)
            {
                //Vector3RGB c(20.0/255.0);
                //double reflec = 0.01;
                //scene.items_[3]->geometry_->f_.o_.z_ = -1.0;
                //scene.items_[17]->geometry_->f_.o_.z_ = 0.0;
                //for (size_t i = 4; i < 17; i++)
                //{
                //    scene.items_[i]->material_->texture_->setColor(c);
                //    scene.items_[i]->material_->reflectiveness_ = reflec;
                //}
                scene.camera_.frame_.o_ = Vector3(1.0, -1.2, 1.0);
                scene.camera_.target_ = Vector3(0.0, 0, 0.5);
                scene.camera_.rotateToTarget();

                scene.items_[0]->material_->texture_->setColor(Vector3RGB(1));
                scene.items_[0]->material_->lightIntensity_ = 1.0;
                scene.items_[0]->geometry_->f_.o_ = Vector3(-1.0, 0.0, 0.3);

                scene.items_[1]->material_->texture_->setColor(Vector3RGB(1));
                scene.items_[1]->material_->lightIntensity_ = 1.0;
                scene.items_[1]->geometry_->f_.o_ = Vector3(0.7, -0.7, 0.3);

                scene.items_[2]->material_->texture_->setColor(Vector3RGB(1));
                scene.items_[2]->material_->lightIntensity_ = 1.0;
                scene.items_[2]->geometry_->f_.o_ = Vector3(0.7, 0.7, 0.3);
            }

            std::cout << "frameId: " << frameId << std::endl;
            //scene.camera_.target_.x_ = frameId*2.1/283.0;
            //scene.camera_.frame_.o_.x_ += 2.4/frameEndFile.back();
            //scene.camera_.frame_.o_.z_ -= 1.4/283.0;
            //scene.camera_.target_.z_ -= 1.4/283.0;
            //scene.camera_.rotateToTarget();
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
            std::string imageFilePath(imageFileBasePath + std::to_string(frameId) + ".bmp");
            window.save(imageFilePath);
            std::cout << PerformanceTracker::instance().describe();
        }
    }
    std::cout << "out of render" << std::endl;
    SDL_Quit();


    std::string videoName("scenarios/turnaroundDisco_" + std::to_string(scene.camera_.resX_) + ".mkv");

    std::string videoCreationCommand("ffmpeg -r 60 -i " + imageFileBasePath + "%d.bmp " + videoName);
    std::cout << "videoCreationCommand.c_str(): " << videoCreationCommand.c_str() << std::endl;
    std::system(videoCreationCommand.c_str());
    std::cout << "Video saved in " << videoName << std::endl;

    return;
}

void sideStepDisco()
{
    std::string sceneJsonName("atalanteStlDisco");

    std::string path = std::string(DATA) + sceneJsonName + ".json";
    if(not fileExists(path))
    {
        StringException error("Error: this file " + path + " does not exist");
        throw error;
    }

    SDL_Init(SDL_INIT_VIDEO);

    SceneLoader sceneLoader(path);
    Scene scene(sceneLoader.sceneData_);
    bool display = sceneLoader.sceneData_.sExData.display;
    Window window(scene.camera_.resX_, scene.camera_.resY_, display);
    Renderer renderer(sceneLoader.sceneData_.rData);

    std::string imageFolderPath(std::string(DATA) + "../images/videoAtalante/individualImages/");
    std::string imageFileBasePath(imageFolderPath + "sideStepDisco_" + std::to_string(scene.camera_.resX_) + "_");

    /***************
    *  Side Step  *
    ***************/
    std::vector<std::string> motionJsonNames{
        "trajectories/patient_000052/repo_lateral_60fps_0_100",
        "trajectories/patient_000052/repo_lateral_60fps_100_200",
        "trajectories/patient_000052/repo_lateral_60fps_200_300",
        "trajectories/patient_000052/repo_lateral_60fps_300_400"
    };
    std::vector<int> frameStartFile{0, 100, 200, 300};
    std::vector<int> frameEndFile{100, 200, 300, 379};

    for (size_t motionFileIndex = 0; motionFileIndex < motionJsonNames.size(); motionFileIndex++)
    {
        std::string motionPath = std::string(DATA) + motionJsonNames[motionFileIndex] + ".json";
        MotionLoader motionLoader(motionPath);
        for (int frameId = frameStartFile[motionFileIndex]; frameId < frameEndFile[motionFileIndex]; frameId++)
        {
            //Disco switch
            if (frameId%50 == 40)
                scene.items_[3]->material_->texture_->switchColors();
            if(frameId == 0)
            {
                scene.camera_.frame_.o_ = Vector3(1.5, 0.1, 1.2);
                scene.camera_.target_ = Vector3(0.2, 0.0, 0.7);
                scene.camera_.rotateToTarget();

                //scene.items_[0]->material_->texture_->setColor(Vector3RGB(1));
                scene.items_[0]->material_->lightIntensity_ = 1.0;
                scene.items_[0]->geometry_->f_.o_ = Vector3(-0.4, 1.0, 1.6);

                //scene.items_[1]->material_->texture_->setColor(Vector3RGB(1));
                scene.items_[1]->material_->lightIntensity_ = 1.0;
                scene.items_[1]->geometry_->f_.o_ = Vector3(0.7, -0.7, 1.3);

                //scene.items_[2]->material_->texture_->setColor(Vector3RGB(1));
                scene.items_[2]->material_->lightIntensity_ = 1.0;
                scene.items_[2]->geometry_->f_.o_ = Vector3(0.7, 0.7, 1.0);
            }

            std::cout << "frameId: " << frameId << std::endl;
            //scene.camera_.target_.x_ = frameId*2.1/283.0;
            //scene.camera_.frame_.o_.x_ += 2.4/frameEndFile.back();
            //scene.camera_.frame_.o_.z_ -= 1.4/283.0;
            //scene.camera_.target_.z_ -= 1.4/283.0;
            //scene.camera_.rotateToTarget();
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
            std::string imageFilePath(imageFileBasePath + std::to_string(frameId) + ".bmp");
            window.save(imageFilePath);
            std::cout << PerformanceTracker::instance().describe();
        }
    }
    std::cout << "out of render" << std::endl;
    SDL_Quit();


    std::string videoName("scenarios/sideStepDisco_" + std::to_string(scene.camera_.resX_) + ".mkv");

    std::string videoCreationCommand("ffmpeg -r 60 -i " + imageFileBasePath + "%d.bmp " + videoName);
    std::cout << "videoCreationCommand.c_str(): " << videoCreationCommand.c_str() << std::endl;
    std::system(videoCreationCommand.c_str());
    std::cout << "Video saved in " << videoName << std::endl;

    return;
}

void backStepDisco()
{
    std::string sceneJsonName("atalanteStlDisco");

    std::string path = std::string(DATA) + sceneJsonName + ".json";
    if(not fileExists(path))
    {
        StringException error("Error: this file " + path + " does not exist");
        throw error;
    }

    SDL_Init(SDL_INIT_VIDEO);

    SceneLoader sceneLoader(path);
    Scene scene(sceneLoader.sceneData_);
    bool display = sceneLoader.sceneData_.sExData.display;
    Window window(scene.camera_.resX_, scene.camera_.resY_, display);
    Renderer renderer(sceneLoader.sceneData_.rData);

    std::string imageFolderPath(std::string(DATA) + "../images/videoAtalante/individualImages/");
    std::string imageFileBasePath(imageFolderPath + "backStepDisco_" + std::to_string(scene.camera_.resX_) + "_");

    /***************
    *  Back Step  *
    ***************/
    std::vector<std::string> motionJsonNames{
        "trajectories/patient_000052/repo_back_60fps_0_100",
        "trajectories/patient_000052/repo_back_60fps_100_200",
        "trajectories/patient_000052/repo_back_60fps_200_300",
        "trajectories/patient_000052/repo_back_60fps_300_400"
    };
    std::vector<int> frameStartFile{0, 100, 200, 300};
    std::vector<int> frameEndFile{100, 200, 300, 379};

    for (size_t motionFileIndex = 0; motionFileIndex < motionJsonNames.size(); motionFileIndex++)
    {
        std::string motionPath = std::string(DATA) + motionJsonNames[motionFileIndex] + ".json";
        MotionLoader motionLoader(motionPath);
        for (int frameId = frameStartFile[motionFileIndex]; frameId < frameEndFile[motionFileIndex]; frameId++)
        {
            //Disco switch
            if (frameId%50 == 25)
                scene.items_[3]->material_->texture_->switchColors();
            if(frameId == 0)
            {
                scene.camera_.frame_.o_ = Vector3(1.5, 0.0, 1.2);
                scene.camera_.target_ = Vector3(0.2, 0.0, 0.7);
                scene.camera_.rotateToTarget();

                //scene.items_[0]->material_->texture_->setColor(Vector3RGB(1));
                scene.items_[0]->material_->lightIntensity_ = 1.0;
                scene.items_[0]->geometry_->f_.o_ = Vector3(0.7, 0.7, 1.0);

                //scene.items_[1]->material_->texture_->setColor(Vector3RGB(1));
                scene.items_[1]->material_->lightIntensity_ = 1.0;
                scene.items_[1]->geometry_->f_.o_ = Vector3(-0.4, 1.0, 1.6);

                //scene.items_[2]->material_->texture_->setColor(Vector3RGB(1));
                scene.items_[2]->material_->lightIntensity_ = 1.0;
                scene.items_[2]->geometry_->f_.o_ = Vector3(0.7, -0.7, 1.3);
            }

            std::cout << "frameId: " << frameId << std::endl;
            //scene.camera_.target_.x_ = frameId*2.1/283.0;
            //scene.camera_.frame_.o_.x_ += 2.4/frameEndFile.back();
            //scene.camera_.frame_.o_.z_ -= 1.4/283.0;
            //scene.camera_.target_.z_ -= 1.4/283.0;
            //scene.camera_.rotateToTarget();
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
            std::string imageFilePath(imageFileBasePath + std::to_string(frameId) + ".bmp");
            window.save(imageFilePath);
            std::cout << PerformanceTracker::instance().describe();
        }
    }
    std::cout << "out of render" << std::endl;
    SDL_Quit();


    std::string videoName("scenarios/backStepDisco_" + std::to_string(scene.camera_.resX_) + ".mkv");

    std::string videoCreationCommand("ffmpeg -r 60 -i " + imageFileBasePath + "%d.bmp " + videoName);
    std::cout << "videoCreationCommand.c_str(): " << videoCreationCommand.c_str() << std::endl;
    std::system(videoCreationCommand.c_str());
    std::cout << "Video saved in " << videoName << std::endl;

    return;
}

void sitDownDisco()
{
    std::string sceneJsonName("atalanteStlDisco");

    std::string path = std::string(DATA) + sceneJsonName + ".json";
    if(not fileExists(path))
    {
        StringException error("Error: this file " + path + " does not exist");
        throw error;
    }

    SDL_Init(SDL_INIT_VIDEO);

    SceneLoader sceneLoader(path);
    Scene scene(sceneLoader.sceneData_);
    bool display = sceneLoader.sceneData_.sExData.display;
    Window window(scene.camera_.resX_, scene.camera_.resY_, display);
    Renderer renderer(sceneLoader.sceneData_.rData);

    std::string imageFolderPath(std::string(DATA) + "../images/videoAtalante/individualImages/");
    std::string imageFileBasePath(imageFolderPath + "sitDownDisco_" + std::to_string(scene.camera_.resX_) + "_");

    /***************
    *  Sit Down  *
    ***************/
    std::vector<std::string> motionJsonNames{
        "trajectories/patient_000052/sit_down_60fps",
    };
    std::vector<int> frameStartFile{0,100};
    std::vector<int> frameEndFile{100,199};

    for (size_t motionFileIndex = 0; motionFileIndex < motionJsonNames.size(); motionFileIndex++)
    {
        std::string motionPath = std::string(DATA) + motionJsonNames[motionFileIndex] + ".json";
        MotionLoader motionLoader(motionPath);
        for (int frameId = frameStartFile[motionFileIndex]; frameId < frameEndFile[motionFileIndex]; frameId++)
        {
            //Disco switch
            if (frameId%50 == 5)
                scene.items_[3]->material_->texture_->switchColors();
            if(frameId == 0)
            {
                scene.ambiantLight_.intensity_ = 0.4;

                scene.camera_.frame_.o_ = Vector3(0.6, -1.2, 0.5);
                scene.camera_.target_ = Vector3(-0.1, 0, 0.65);
                scene.camera_.rotateToTarget();

                scene.items_[0]->material_->texture_->setColor(Vector3RGB(1));
                scene.items_[0]->material_->lightIntensity_ = 1.0;
                scene.items_[0]->geometry_->f_.o_ = Vector3(-1.0, 0.0, 1.8);

                scene.items_[1]->material_->texture_->setColor(Vector3RGB(1));
                scene.items_[1]->material_->lightIntensity_ = 1.0;
                scene.items_[1]->geometry_->f_.o_ = Vector3(0.7, -0.7, 1.8);

                scene.items_[2]->material_->texture_->setColor(Vector3RGB(1));
                scene.items_[2]->material_->lightIntensity_ = 1.0;
                scene.items_[2]->geometry_->f_.o_ = Vector3(0.7, 0.7, 1.8);
            }

            std::cout << "frameId: " << frameId << std::endl;
            //scene.camera_.target_.x_ = frameId*2.1/283.0;
            //scene.camera_.frame_.o_.x_ += 2.4/frameEndFile.back();
            //scene.camera_.frame_.o_.z_ -= 1.4/283.0;
            //scene.camera_.target_.z_ -= 1.4/283.0;
            //scene.camera_.rotateToTarget();
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
            std::string imageFilePath(imageFileBasePath + std::to_string(frameId) + ".bmp");
            window.save(imageFilePath);
            std::cout << PerformanceTracker::instance().describe();
        }
    }
    std::cout << "out of render" << std::endl;
    SDL_Quit();


    std::string videoName("scenarios/sitDownDisco_" + std::to_string(scene.camera_.resX_) + ".mkv");

    std::string videoCreationCommand("ffmpeg -r 60 -i " + imageFileBasePath + "%d.bmp " + videoName);
    std::cout << "videoCreationCommand.c_str(): " << videoCreationCommand.c_str() << std::endl;
    std::system(videoCreationCommand.c_str());
    std::cout << "Video saved in " << videoName << std::endl;

    return;
}

void standUpDisco()
{
    std::string sceneJsonName("atalanteStlDisco");

    std::string path = std::string(DATA) + sceneJsonName + ".json";
    if(not fileExists(path))
    {
        StringException error("Error: this file " + path + " does not exist");
        throw error;
    }

    SDL_Init(SDL_INIT_VIDEO);

    SceneLoader sceneLoader(path);
    Scene scene(sceneLoader.sceneData_);
    bool display = sceneLoader.sceneData_.sExData.display;
    Window window(scene.camera_.resX_, scene.camera_.resY_, display);
    Renderer renderer(sceneLoader.sceneData_.rData);

    std::string imageFolderPath(std::string(DATA) + "../images/videoAtalante/individualImages/");
    std::string imageFileBasePath(imageFolderPath + "standUpDisco_" + std::to_string(scene.camera_.resX_) + "_");

    /***************
    *  Stand Up  *
    ***************/
    std::vector<std::string> motionJsonNames{
        "trajectories/patient_000052/stand_up_60fps",
    };
    std::vector<int> frameStartFile{0};
    std::vector<int> frameEndFile{99};

    for (size_t motionFileIndex = 0; motionFileIndex < motionJsonNames.size(); motionFileIndex++)
    {
        std::string motionPath = std::string(DATA) + motionJsonNames[motionFileIndex] + ".json";
        MotionLoader motionLoader(motionPath);
        for (int frameId = frameStartFile[motionFileIndex]; frameId < frameEndFile[motionFileIndex]; frameId++)
        {
            //Disco switch
            if (frameId%50 == 5)
                scene.items_[3]->material_->texture_->switchColors();
            if(frameId == 0)
            {
                scene.ambiantLight_.intensity_ = 0.4;

                scene.camera_.frame_.o_ = Vector3(0.6, -1.2, 0.5);
                scene.camera_.target_ = Vector3(-0.1, 0, 0.65);
                scene.camera_.rotateToTarget();

                scene.items_[0]->material_->texture_->setColor(Vector3RGB(1));
                scene.items_[0]->material_->lightIntensity_ = 1.0;
                scene.items_[0]->geometry_->f_.o_ = Vector3(-1.0, 0.0, 1.8);

                scene.items_[1]->material_->texture_->setColor(Vector3RGB(1));
                scene.items_[1]->material_->lightIntensity_ = 1.0;
                scene.items_[1]->geometry_->f_.o_ = Vector3(0.7, -0.7, 1.8);

                scene.items_[2]->material_->texture_->setColor(Vector3RGB(1));
                scene.items_[2]->material_->lightIntensity_ = 1.0;
                scene.items_[2]->geometry_->f_.o_ = Vector3(0.7, 0.7, 1.8);
            }

            std::cout << "frameId: " << frameId << std::endl;
            //scene.camera_.target_.x_ = frameId*2.1/283.0;
            //scene.camera_.frame_.o_.x_ += 2.4/frameEndFile.back();
            //scene.camera_.frame_.o_.z_ -= 1.4/283.0;
            //scene.camera_.target_.z_ -= 1.4/283.0;
            //scene.camera_.rotateToTarget();
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
            std::string imageFilePath(imageFileBasePath + std::to_string(frameId) + ".bmp");
            window.save(imageFilePath);
            std::cout << PerformanceTracker::instance().describe();
        }
    }
    std::cout << "out of render" << std::endl;
    SDL_Quit();


    std::string videoName("scenarios/standUpDisco_" + std::to_string(scene.camera_.resX_) + ".mkv");

    std::string videoCreationCommand("ffmpeg -r 60 -i " + imageFileBasePath + "%d.bmp " + videoName);
    std::cout << "videoCreationCommand.c_str(): " << videoCreationCommand.c_str() << std::endl;
    std::system(videoCreationCommand.c_str());
    std::cout << "Video saved in " << videoName << std::endl;

    return;
}
