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

#include <vector>

double deg2rad(double d);
double rad2deg(double d);

bool solve2ndOrderEq(double a, double b, double c, double& x0, double& x1);

bool fileExists(const std::string& s);

void printManual();
void printMat(std::vector<std::vector<int> > m);
