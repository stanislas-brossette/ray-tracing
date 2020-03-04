#pragma once
#include <iostream>
#include <SDL.h>
#include <SDL_image.h>
#include <SDL_ttf.h>
#include <SDL_mixer.h>

#include "Pixel.hh"

class Window
{
public:
  Window ();
  Window (int resX, int resY);
  void addPixel(const Pixel& p);
  void render();
  ~Window ();

private:
  SDL_Window *window_;
  SDL_Renderer *renderer_;
  int resX_;
  int resY_;
  int renderCounter_;
};

