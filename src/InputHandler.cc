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
    double transIncrement = 0.5;

    switch (event.type)
    {
    case SDL_QUIT:
        quit = true;
        break;
    case SDL_MOUSEBUTTONDOWN:
        int mouseX, mouseY;
        SDL_GetMouseState(&mouseX, &mouseY);
        switch (event.button.button)
        {
            case SDL_BUTTON_LEFT:
                scene.castPrimaryRayAt(mouseX, mouseY);
                break;
            case SDL_BUTTON_RIGHT:
                Pixel pix;
                LightRay lr;
                scene.camera_.castRayAt(mouseX, scene.camera_.resY_-mouseY, lr, pix);
                size_t impactItemIndex;
                Vector3 impactPoint;
                Vector3 impactNormal;
                double impactDist = 1e9;
                scene.findFirstImpact(lr, impactItemIndex, impactPoint, impactNormal, impactDist);
                scene.camera_.target_ = impactPoint;
                std::cout << "=== new target: " << impactPoint << " ===" << std::endl;
                scene.camera_.rotateToTarget();
                needRender = true;
                break;
        }
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
                if(rotationMode)
                    std::cout << "=== Rotation mode activated ===" << std::endl;
                else
                    std::cout << "=== Rotation mode disabled ===" << std::endl;
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
                if(lockTarget)
                    std::cout << "=== Target Lock mode activated ===" << std::endl;
                else
                    std::cout << "=== Target Lock mode disabled ===" << std::endl;
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
