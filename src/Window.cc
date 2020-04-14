#include "Window.hh"

Window::Window()
    : window_(nullptr),
      renderer_(nullptr),
      resX_(10),
      resY_(10),
      renderCounter_(0),
      display_(true)
{
    resizeImage(resX_, resY_);
}

Window::Window(int resX, int resY, bool display)
    : window_(nullptr),
      renderer_(nullptr),
      resX_(resX),
      resY_(resY),
      display_(display)
{
    resizeImage(resX_, resY_);

    if(display_)
    {
        Uint32 flags = SDL_WINDOW_SHOWN;

        // Set up window
        window_ = SDL_CreateWindow("RayTracing", SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED, resX_, resY_, flags);
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
}

void Window::addPixel(const Pixel& p)
{
    if(display_)
    {
        SDL_SetRenderDrawColor(renderer_, 255*p.r(), 255*p.g(), 255*p.b(), 255);
        SDL_RenderDrawPoint(renderer_, p.x_, resY_-p.y_);
    }
    image_[p.x_][resY_-p.y_][0] = (unsigned char)(255*p.b());
    image_[p.x_][resY_-p.y_][1] = (unsigned char)(255*p.g());
    image_[p.x_][resY_-p.y_][2] = (unsigned char)(255*p.r());
}

void Window::addPixels(const std::vector<Pixel>& v)
{
    for (size_t i = 0; i < v.size(); i++)
    {
        if(display_)
        {
            SDL_SetRenderDrawColor(renderer_, 255*v[i].r(), 255*v[i].g(), 255*v[i].b(), 255);
            SDL_RenderDrawPoint(renderer_, v[i].x_, resY_-v[i].y_);
        }
        image_[v[i].x_][resY_-v[i].y_][0] = (unsigned char)(255*v[i].b());
        image_[v[i].x_][resY_-v[i].y_][1] = (unsigned char)(255*v[i].g());
        image_[v[i].x_][resY_-v[i].y_][2] = (unsigned char)(255*v[i].r());
    }
}

void Window::clear()
{
    for (size_t i = 0; i < resX_; i++)
    {
        for (size_t j = 0; j < resY_; j++)
        {
            if(display_)
            {
                SDL_SetRenderDrawColor(renderer_, 0, 0, 0, 0);
                SDL_RenderDrawPoint(renderer_, i, j);
            }
            for (size_t k = 0; k < 3; k++)
            {
                image_[i][j][k] = (unsigned char)(0);
            }
        }
    }
    if(display_)
    {
        SDL_RenderClear(renderer_);
    }
    render();
}

void Window::render()
{
    // Render to screen
    if(display_)
        SDL_RenderPresent(renderer_);
}

Window::~Window()
{
    if(display_)
    {
        SDL_DestroyRenderer(renderer_);
        SDL_DestroyWindow(window_);
    }
}

void Window::save(std::string filepath)
{
    char fpath[filepath.size() + 1];
    strcpy(fpath, filepath.c_str());
    generateBitmapImage(image_, resX_, resY_, fpath);
}

void Window::changeResolution(int resX, int resY)
{
    resX_ = resX;
    resY_ = resY;
    if(display_)
        SDL_SetWindowSize(window_, resX_, resY_);
    resizeImage(resX_, resY);
}

void Window::resizeImage(int resX, int resY)
{
    image_.resize(resX + 1);
    for (size_t i = 0; i < resX + 1; i++)
    {
        image_[i].resize(resY + 1);
        for (size_t j = 0; j < resY + 1; j++)
        {
            image_[i][j].resize(3);
            for (size_t k = 0; k < 3; k++)
            {
                image_[i][j][k] = (unsigned char)(0);
            }
        }
    }
}
