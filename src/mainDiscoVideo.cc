#include <iostream>
#include <chrono>
#include <vector>
#include <sstream>
#include <cstdlib>

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
#include "scenarios.hh"

int main(int argc, char *argv[])
{
    try
    {
        //transition(); //OK
        //footRollingTraveling(); //OK
        //footRollingPan(); //OK
        flatFootDisco(); //OK
        //footRollingDisco();
        //turnaroundDisco();
        //sideStepDisco();
        //backStepDisco();
        //sitDownDisco();
        //standUpDisco();
    }
    catch(StringException e)
    {
        std::cout << e.what() << std::endl;
    }
}


