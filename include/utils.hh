#pragma once

#ifdef DATA_FOLDER
#define DATA DATA_FOLDER
#else
#define DATA ""
#endif

double deg2rad(double d);
double rad2deg(double d);

bool solve2ndOrderEq(double a, double b, double c, double& x0, double& x1);
