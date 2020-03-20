#include <iostream>

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

void printManual()
{
    std::cout << "This in RayTracings manual:" << std::endl;
    std::cout << "q: quit" << std::endl;
    std::cout << "right: move camera to right" << std::endl;
    std::cout << "left: move camera to left" << std::endl;
    std::cout << "up: move camera to up" << std::endl;
    std::cout << "down: move camera to down" << std::endl;
    std::cout << "F5: refresh window" << std::endl;
    std::cout << "c: clear window" << std::endl;
    std::cout << "+(number pad): Increase resolution by 10%" << std::endl;
    std::cout << "-(number pad): Decrease resolution by 10%" << std::endl;
}
