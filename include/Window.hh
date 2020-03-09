#pragma once
#include <iostream>
#include <SDL.h>
#include <SDL_image.h>
#include <SDL_ttf.h>
#include <SDL_mixer.h>
#include <vector>

#include "Pixel.hh"

class Window
{
public:
    Window ();
    Window (int resX, int resY);
    void addPixel(const Pixel& p);
    void addPixels(const std::vector<Pixel>& v);
    void render();
    void renderConvolut();
    void clear();
    ~Window ();

private:
    SDL_Window *window_;
    SDL_Renderer *renderer_;
    int resX_;
    int resY_;
    int renderCounter_;
    std::vector<std::vector<Pixel> > originalPixels_;
    std::vector<std::vector<Pixel> > convolutPixels_;
    int nPixels_;
};

