#include "Window.hh"

Window::Window()
    : window_(nullptr),
      renderer_(nullptr),
      resX_(10),
      resY_(10),
      renderCounter_(0)
{
}

Window::Window(int resX, int resY)
    : window_(nullptr),
      renderer_(nullptr),
      resX_(resX),
      resY_(resY)
{
    Uint32 flags = SDL_WINDOW_SHOWN;

    // Set up window
    window_ = SDL_CreateWindow("RayTracing", SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED, resX, resY, flags);
    if(!window_) {
        std::cerr << SDL_GetError() << std::endl;
    }

    // Set up renderer
    flags = 0;
    flags |= SDL_RENDERER_ACCELERATED;
    renderer_ = SDL_CreateRenderer(window_, -1, flags);
    SDL_SetRenderDrawBlendMode(renderer_, SDL_BLENDMODE_BLEND);

    if(renderer_ == nullptr) {
        std::cerr << SDL_GetError() << std::endl;
    }
}

void Window::addPixel(const Pixel& p)
{
    SDL_SetRenderDrawColor(renderer_, p.r_, p.g_, p.b_, p.a_);
    SDL_RenderDrawPoint(renderer_, p.x_, resY_-p.y_);
}

void Window::addPixels(const std::vector<Pixel>& v)
{
    for (size_t i = 0; i < v.size(); i++)
    {
        SDL_SetRenderDrawColor(renderer_, v[i].r_, v[i].g_, v[i].b_, v[i].a_);
        SDL_RenderDrawPoint(renderer_, v[i].x_, resY_-v[i].y_);
    }
}

void Window::clear()
{
    SDL_RenderClear(renderer_);
}

void Window::render()
{
    // Render to screen
    SDL_RenderPresent(renderer_);
}

Window::~Window()
{
    SDL_DestroyRenderer(renderer_);
    SDL_DestroyWindow(window_);
}

bool Window::save(std::string filepath)
{
    SDL_Surface* saveSurface = NULL;
    SDL_Surface* infoSurface = NULL;
    infoSurface = SDL_GetWindowSurface(window_);
    if (infoSurface == NULL)
    {
        std::cerr << "Failed to create info surface from window in saveScreenshotBMP(string), SDL_GetError() - " << SDL_GetError() << "\n";
    }
    else
    {
        unsigned char * pixels = new (std::nothrow) unsigned char[infoSurface->w * infoSurface->h * infoSurface->format->BytesPerPixel];
        if (pixels == 0)
        {
            std::cerr << "Unable to allocate memory for screenshot pixel data buffer!\n";
            return false;
        }
        else
        {
            if (SDL_RenderReadPixels(renderer_, &infoSurface->clip_rect, infoSurface->format->format, pixels, infoSurface->w * infoSurface->format->BytesPerPixel) != 0)
            {
                std::cerr << "Failed to read pixel data from SDL_Renderer object. SDL_GetError() - " << SDL_GetError() << "\n";
                delete[] pixels;
                return false;
            }
            else
            {
                saveSurface = SDL_CreateRGBSurfaceFrom(pixels, infoSurface->w, infoSurface->h, infoSurface->format->BitsPerPixel, infoSurface->w * infoSurface->format->BytesPerPixel, infoSurface->format->Rmask, infoSurface->format->Gmask, infoSurface->format->Bmask, infoSurface->format->Amask);
                if (saveSurface == NULL)
                {
                    std::cerr << "Couldn't create SDL_Surface from renderer pixel data. SDL_GetError() - " << SDL_GetError() << "\n";
                    delete[] pixels;
                    return false;
                }
                SDL_SaveBMP(saveSurface, filepath.c_str());
                SDL_FreeSurface(saveSurface);
                saveSurface = NULL;
            }
            delete[] pixels;
        }
        SDL_FreeSurface(infoSurface);
        infoSurface = NULL;
    }
    return true;
}
