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
    std::cout << "PageUp: Move camera up\n";
    std::cout << "PageDown: Move camera down\n";
    std::cout << "r: Toggle rotation mode (may be buggy)\n";
    std::cout << "l: Reset camera rotation to center on target and upGuide\n";
    std::cout << "F5: Refresh window\n";
    std::cout << "c: Clear window\n";
    std::cout << "s: Toggle simplified render\n";
    std::cout << "+(number pad): Increase resolution by 20%\n";
    std::cout << "-(number pad): Decrease resolution by 20%\n";
    std::cout << "leftClick: cast ray on pixel and get its trace\n";
    std::cout << "rightClick: change target to clicked 3D point\n";
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

double INFINITY_d()
{
    return std::numeric_limits<double>::infinity();
}

float INFINITY_f()
{
    return std::numeric_limits<float>::infinity();
}

Uint32 getpixel(SDL_Surface *surface, int x, int y)
{
    int bpp = surface->format->BytesPerPixel;
    /* Here p is the address to the pixel we want to retrieve */
    Uint8 *p = (Uint8 *)surface->pixels + y * surface->pitch + x * bpp;

    switch(bpp) {
    case 1:
        return *p;
        break;

    case 2:
        return *(Uint16 *)p;
        break;

    case 3:
        if(SDL_BYTEORDER == SDL_BIG_ENDIAN)
            return p[0] << 16 | p[1] << 8 | p[2];
        else
            return p[0] | p[1] << 8 | p[2] << 16;
        break;

    case 4:
        return *(Uint32 *)p;
        break;

    default:
        return 0;       /* shouldn't happen, but avoids warnings */
    }
}

std::string multiTab(int n)
{
    std::stringstream ss;
    for (size_t i = 0; i < n; i++)
    {
        ss << "\t";
    }
    return ss.str();
}

std::vector<std::string> split(const std::string& input, const std::string& regex) {
    // passing -1 as the submatch index parameter performs splitting
    std::regex re(regex);
    std::sregex_token_iterator
        first{input.begin(), input.end(), re, -1},
        last;
    return {first, last};
}
