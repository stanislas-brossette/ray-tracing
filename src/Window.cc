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

void Window::render()
{
  // Clear screen
  SDL_RenderClear(renderer_);

  // Draw
  SDL_SetRenderDrawColor(renderer_, 0x00, 0x00, 0x00, 0x00);

  for (size_t i = 0; i < resX_; i++)
  {
    for (size_t j = 0; j < resY_; j++)
    {
      SDL_SetRenderDrawColor(renderer_, i%255, j%255, 0, 255);
      SDL_RenderDrawPoint(renderer_, i, j);
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
