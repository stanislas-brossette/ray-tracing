#pragma once
#include <iostream>

class Pixel
{
public:
  Pixel(int x, int y, int a, int r, int g, int b);
  ~Pixel();
  int x_;
  int y_;
  int a_;
  int r_;
  int g_;
  int b_;
};
