#include "Window.hh"

Window::Window()
    : window_(nullptr),
      renderer_(nullptr),
      resX_(10),
      resY_(10),
      renderCounter_(0),
      originalPixels_(resX_),
      convolutPixels_(resX_)
{
    for (size_t i = 0; i < resX_; i++)
    {
        originalPixels_[i] = std::vector<Pixel>(resY_);
        convolutPixels_[i] = std::vector<Pixel>(resY_);
    }
}

Window::Window(int resX, int resY)
    : window_(nullptr),
      renderer_(nullptr),
      resX_(resX),
      resY_(resY),
      originalPixels_(resX_),
      convolutPixels_(resX_)
{
    for (size_t i = 0; i < resX_; i++)
    {
        originalPixels_[i] = std::vector<Pixel>(resY_);
        convolutPixels_[i] = std::vector<Pixel>(resY_);
    }
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
    int px = p.x_;
    int py = p.y_;
    originalPixels_[px][resY_-py].a_ = p.a_;
    originalPixels_[px][resY_-py].r_ = p.r_;
    originalPixels_[px][resY_-py].g_ = p.g_;
    originalPixels_[px][resY_-py].b_ = p.b_;
    nPixels_++;

    SDL_SetRenderDrawColor(renderer_, p.r_, p.g_, p.b_, p.a_);
    SDL_RenderDrawPoint(renderer_, p.x_, resY_-p.y_);
}

void Window::addPixels(const std::vector<Pixel>& v)
{
    for (size_t i = 0; i < v.size(); i++)
    {
        originalPixels_[v.at(i).x_][resY_-v.at(i).y_] = v.at(i);
        nPixels_++;
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

void Window::renderConvolut()
{
    for (size_t i = 1; i < resX_-1; i++)
    {
        for (size_t j = 1; j < resY_-1; j++)
        {
            convolutPixels_[i][j].a_ = 1/9*(originalPixels_[i-1][j-1].a_ + originalPixels_[i-1][j].a_ + originalPixels_[i-1][j+1].a_ + originalPixels_[i][j-1].a_ + originalPixels_[i][j].a_ + originalPixels_[i][j+1].a_ + originalPixels_[i+1][j-1].a_ + originalPixels_[i+1][j].a_ + originalPixels_[i+1][j+1].a_);
            convolutPixels_[i][j].r_ = 1/9*(originalPixels_[i-1][j-1].r_ + originalPixels_[i-1][j].r_ + originalPixels_[i-1][j+1].r_ + originalPixels_[i][j-1].r_ + originalPixels_[i][j].r_ + originalPixels_[i][j+1].r_ + originalPixels_[i+1][j-1].r_ + originalPixels_[i+1][j].r_ + originalPixels_[i+1][j+1].r_);
            convolutPixels_[i][j].g_ = 1/9*(originalPixels_[i-1][j-1].g_ + originalPixels_[i-1][j].g_ + originalPixels_[i-1][j+1].g_ + originalPixels_[i][j-1].g_ + originalPixels_[i][j].g_ + originalPixels_[i][j+1].g_ + originalPixels_[i+1][j-1].g_ + originalPixels_[i+1][j].g_ + originalPixels_[i+1][j+1].g_);
            convolutPixels_[i][j].b_ = 1/9*(originalPixels_[i-1][j-1].b_ + originalPixels_[i-1][j].b_ + originalPixels_[i-1][j+1].b_ + originalPixels_[i][j-1].b_ + originalPixels_[i][j].b_ + originalPixels_[i][j+1].b_ + originalPixels_[i+1][j-1].b_ + originalPixels_[i+1][j].b_ + originalPixels_[i+1][j+1].b_);
        }
    }

    for (size_t i = 1; i < resX_-1; i++)
    {
        for (size_t j = 1; j < resY_-1; j++)
        {
            Pixel& p = convolutPixels_[i][j];
            SDL_SetRenderDrawColor(renderer_, p.r_, p.g_, p.b_, p.a_);
            SDL_RenderDrawPoint(renderer_, p.x_, resY_-p.y_);
        }
    }

    // Render to screen
    SDL_RenderPresent(renderer_);
}

Window::~Window()
{
    SDL_DestroyRenderer(renderer_);
    SDL_DestroyWindow(window_);
}
