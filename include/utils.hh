#pragma once

#ifdef DATA_FOLDER
#define DATA DATA_FOLDER
#else
#define DATA ""
#endif

#ifdef UNIT_DATA_FOLDER
#define UNIT_DATA UNIT_DATA_FOLDER
#else
#define UNIT_DATA ""
#endif

#ifdef MESHES_FOLDER
#define MESHES MESHES_FOLDER
#else
#define MESHES ""
#endif

#ifdef IMAGES_FOLDER
#define IMAGES IMAGES_FOLDER
#else
#define IMAGES ""
#endif

#include <vector>
#include <limits>
#include <sstream>
#include <string>

#include <SDL.h>

double deg2rad(double d);
double rad2deg(double d);

bool solve2ndOrderEq(double a, double b, double c, double& x0, double& x1);

bool fileExists(const std::string& s);

void printManual();
void printMat(std::vector<std::vector<int> > m);

double INFINITY_d();
float INFINITY_f();

Uint32 getpixel(SDL_Surface *surface, int x, int y);

std::string multiTab(int n);
