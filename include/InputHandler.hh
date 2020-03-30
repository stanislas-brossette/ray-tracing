#pragma once
#include "Scene.hh"
#include "Window.hh"

struct InputHandler
{
public:
    InputHandler ();
    virtual ~InputHandler ();
    void handleInputs(Window& window, Scene& scene);

    double rotIncrement = 5;
    double transIncrement = 0.1;

    bool quit;
    bool lockTarget;
    bool rotationMode;
    bool needRender;
    bool needReload;
};
