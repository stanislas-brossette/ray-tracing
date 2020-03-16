#include <sys/stat.h>
#include <cmath>
#include <string>
#include "utils.hh"

double deg2rad(double d)
{
    return d * M_PI/180;
}

double rad2deg(double d)
{
    return d * 180/M_PI;
}

bool solve2ndOrderEq(double a, double b, double c, double& x0, double& x1)
{
    double d = b*b - 4*a*c;
    if (d < 0) //No solution, the light ray does not impact the infinite cylinder
    {
        return false;
    }

    double delta = std::sqrt(d);
    x0 = (-b-delta)/(2*a);
    x1 = (-b+delta)/(2*a);
    return true;
}

bool fileExists(const std::string& s)
{
    struct stat buffer;
    return (stat (s.c_str(), &buffer) == 0);
}
