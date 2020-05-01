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


int main(int argc, char *argv[])
{
    try
    {
        printManual();
        std::string sceneJsonName("atalanteStlDisco");
        if(argc > 1)
            sceneJsonName = std::string(argv[1]);

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
            scene.items_[i]->geometry_->f_.o_  = motionLoader.get(50, scene.items_[i]->name_, "pos");
            scene.items_[i]->geometry_->f_.vx_ = motionLoader.get(50, scene.items_[i]->name_, "vx");
            scene.items_[i]->geometry_->f_.vy_ = motionLoader.get(50, scene.items_[i]->name_, "vy");
            scene.items_[i]->geometry_->f_.vz_ = motionLoader.get(50, scene.items_[i]->name_, "vz");
        }

        for (int frameId = 0; frameId < 360; frameId++)
        {
            //Disco switch
            if (frameId%20 == 5)
                scene.items_[3]->material_->texture_->switchColors();
            std::cout << "frameId: " << frameId << std::endl;

            //Rotate camera
            scene.camera_.frame_.o_ = Vector3(0.04473088918756667, -0.023698960694729244, 0.9304836521315684)
                + Vector3(1.5*std::cos(3.14159/180*2*frameId), 1.5*std::sin(3.14159/180*2*frameId), 0.2);
            scene.camera_.rotateToTarget();

            //Change robot color
            Vector3RGB c((30.0 + (230.0-30.0)*frameId/180.0)/255.0);
            double reflec = 0.01 + (0.3-0.01)*frameId/180.0;
            for (size_t i = 4; i < 17; i++)
            {
                scene.items_[i]->material_->texture_->setColor(c);
                scene.items_[i]->material_->reflectiveness_ = reflec;
            }

            if(frameId == 0)
            {
                scene.items_[0]->material_->texture_->setColor(Vector3RGB(240.0/255.0, 240.0/255.0, 240.0/255.0));
                scene.items_[1]->material_->texture_->setColor(Vector3RGB(240.0/255.0, 240.0/255.0, 240.0/255.0));
                scene.items_[2]->material_->texture_->setColor(Vector3RGB(240.0/255.0, 240.0/255.0, 240.0/255.0));
            }
            //Change lights
            if(frameId > 140 and frameId < 220)
            {
                if(frameId < 180)
                {
                    scene.items_[0]->material_->lightIntensity_ -= 0.0225;
                    scene.items_[1]->material_->lightIntensity_ -= 0.0225;
                    scene.items_[2]->material_->lightIntensity_ -= 0.0225;
                    scene.ambiantLight_.intensity_ -= 0.0045;
                }
                else
                {
                    scene.items_[0]->material_->lightIntensity_ += 0.0225;
                    scene.items_[1]->material_->lightIntensity_ += 0.0225;
                    scene.items_[2]->material_->lightIntensity_ += 0.0225;
                    scene.ambiantLight_.intensity_ += 0.0045;
                }
                if(frameId == 180)
                {
                    scene.items_[0]->material_->texture_->setColor(Vector3RGB(15.0/255.0, 192.0/255.0, 252.0/255.0));
                    scene.items_[1]->material_->texture_->setColor(Vector3RGB(255.0/255.0, 29.0/255.0, 175.0/255.0));
                    scene.items_[2]->material_->texture_->setColor(Vector3RGB(212.0/255.0, 255.0/255.0, 71.0/255));
                }

            }


            //Exchange checker and disco floor
            if(frameId == 180)
            {
                scene.items_[3]->geometry_->f_.o_.z_ = 0.0;
                scene.items_[17]->geometry_->f_.o_.z_ = -1.0;
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

        std::string playCommand("vlc " + videoName);
        std::system(playCommand.c_str());



        return 0;
    }
    catch(StringException& s)
    {
        std::cout << s.what() << std::endl;
    }
}


