#pragma once
#include <cmath>
#include "textures/Texture.hh"
#include "utils.hh"

class Image : public Texture
{
public:
    Image (const std::string& path, double sizeX, double sizeY);
    virtual ~Image () = default;
    Vector3RGB color(double x, double y) const;
    void switchColors();
    void setColor(const Vector3RGB& c);

private:
    SDL_Surface* image_;
    std::string path_;
    double sizeX_;
    double sizeY_;
};

