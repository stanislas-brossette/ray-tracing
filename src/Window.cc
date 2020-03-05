#include "Window.hh"

Window::Window():
  window_(nullptr),
  renderer_(nullptr),
  resX_(10),
  resY_(10),
  renderCounter_(0)
{
}

Window::Window(int resX, int resY):
  window_(nullptr),
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
    if(renderer_ == nullptr) {
        std::cerr << SDL_GetError() << std::endl;
    }
}

void Window::addPixel(const Pixel& p)
{
  SDL_SetRenderDrawColor(renderer_, p.r_, p.g_, p.b_, p.a_);
  SDL_RenderDrawPoint(renderer_, p.x_, p.y_);
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
