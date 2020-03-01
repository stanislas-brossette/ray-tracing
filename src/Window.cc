#include "Window.hh"

Window::Window():
  window_(nullptr),
  renderer_(nullptr),
  resX_(10),
  resY_(10)
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
    window_ = SDL_CreateWindow("RayCasting", SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED, resX, resY, flags);
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

void Window::render(const Pixel& p)
{
  // Clear screen
  //SDL_RenderClear(renderer_);

  // Draw
  std::cout << "Rendering " << p.describe() << std::endl;
  SDL_SetRenderDrawColor(renderer_, p.a_, p.r_, p.g_, p.b_);
  SDL_RenderDrawPoint(renderer_, p.x_, p.y_);

	// Render to screen
	SDL_RenderPresent(renderer_);
}

Window::~Window()
{
  SDL_DestroyRenderer(renderer_);
  SDL_DestroyWindow(window_);
}
