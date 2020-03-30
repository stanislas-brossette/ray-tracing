#include "InputHandler.hh"

InputHandler::InputHandler()
    : quit(false),
    lockTarget(false),
    rotationMode(false),
    needRender(true),
    needReload(false),
    rotIncrement(5),
    transIncrement(0.1)
{
}

InputHandler::~InputHandler()
{
}


void InputHandler::handleInputs(Window& window, Scene& scene)
{
    SDL_Event event;
    SDL_WaitEvent(&event);
    SDL_Delay(5);

    double rotIncrement = 5;
    double transIncrement = 0.1;

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
                needReload = true;
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
            // Toggle free rotation mode
            case SDLK_r:
                lockTarget = false;
                rotationMode = not rotationMode;
                break;
            // Reset camera to target
            case SDLK_t:
                scene.camera_.rotateToTarget();
                needRender = true;
                break;
            // Lock target
            case SDLK_l:
                rotationMode = false;
                lockTarget = not lockTarget;
                scene.camera_.rotateToTarget();
                needRender = true;
                break;
            // Move camera left right up down
            case SDLK_LEFT:
                if(rotationMode)
                    scene.camera_.frame_.yaw(rotIncrement);
                else
                    scene.translateCameraLocal(-transIncrement, 0.0, 0.0);
                if(lockTarget)
                    scene.camera_.rotateToTarget();
                needRender = true;
                break;
            case SDLK_RIGHT:
                if(rotationMode)
                    scene.camera_.frame_.yaw(-rotIncrement);
                else
                    scene.translateCameraLocal(transIncrement, 0.0, 0.0);
                if(lockTarget)
                    scene.camera_.rotateToTarget();
                needRender = true;
                break;
            case SDLK_UP:
                if(rotationMode)
                    scene.camera_.frame_.pitch(rotIncrement);
                else
                    scene.translateCameraLocal(0.0, transIncrement, 0.0);
                if(lockTarget)
                    scene.camera_.rotateToTarget();
                needRender = true;
                break;
            case SDLK_DOWN:
                if(rotationMode)
                    scene.camera_.frame_.pitch(-rotIncrement);
                else
                    scene.translateCameraLocal(0.0, -transIncrement, 0.0);
                if(lockTarget)
                    scene.camera_.rotateToTarget();
                needRender = true;
                break;
            case SDLK_PAGEUP:
                if(rotationMode)
                    scene.camera_.frame_.roll(rotIncrement);
                else
                    scene.translateCameraLocal(0.0, 0.0, transIncrement);
                if(lockTarget)
                    scene.camera_.rotateToTarget();
                needRender = true;
                break;
            case SDLK_PAGEDOWN:
                if(rotationMode)
                    scene.camera_.frame_.roll(-rotIncrement);
                else
                    scene.translateCameraLocal(0.0, 0.0, -transIncrement);
                if(lockTarget)
                    scene.camera_.rotateToTarget();
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
