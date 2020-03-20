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
    std::cout << "==================================================\n";
    std::cout << "This in RayTracings manual:\n";
    std::cout << "q: Quit\n";
    std::cout << "Right: Move camera to right\n";
    std::cout << "Left: Move camera to left\n";
    std::cout << "Up: Move camera forward\n";
    std::cout << "Down: Move camera backward\n";
    std::cout << "PageUp: Move camera forward\n";
    std::cout << "PageDown: Move camera backward\n";
    std::cout << "F5: Refresh window\n";
    std::cout << "c: Clear window\n";
    std::cout << "s: Toggle simplified render\n";
    std::cout << "+(number pad): Increase resolution by 20%\n";
    std::cout << "-(number pad): Decrease resolution by 20%\n";
    std::cout << "==================================================" << std::endl;
}

void printMat(std::vector<std::vector<int> > m)
{
    std::cout << "rows: " << m.size() << std::endl;
    std::cout << "cols: " << m[0].size() << std::endl;
    for (size_t i = 0; i < m.size(); i++)
    {
        std::cout << "[";
        for (size_t j = 0; j < m[0].size(); j++)
        {
            std::cout << m[i][j] << ",";
        }
        std::cout << "]\n";
    }
    std::cout << std::endl;
}
