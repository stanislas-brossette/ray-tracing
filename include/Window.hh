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
    void clear();
    ~Window ();
    int resX() const{return resX_;};
    int resY() const{return resY_;};
    int nPixels() const{return resX_*resY_;};
    bool save(std::string filepath = "image.bmp");
    SDL_Window* win(){return window_;};
    void changeResolution(int resX, int resY);

private:
    SDL_Window *window_;
    SDL_Renderer *renderer_;
    int resX_;
    int resY_;
    int renderCounter_;
};

