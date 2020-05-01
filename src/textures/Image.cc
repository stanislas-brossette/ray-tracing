#include "textures/Image.hh"
#include "StringException.hh"

Image::Image (const std::string& path, double sizeX, double sizeY)
    : Texture(),
    path_(path),
    sizeX_(sizeX),
    sizeY_(sizeY),
    image_(nullptr)
{
    image_ = SDL_LoadBMP(path.c_str());
    if(not image_)
        image_ = SDL_LoadBMP((IMAGES_FOLDER + path).c_str());
    if(not image_)
    {
        StringException error("Failed to load " + path);
        throw error;
    }
}

Vector3RGB Image::color(double x, double y) const
{
    Vector3RGB c(0,0,0);
    if(not(-sizeX_/2 <= x and x <= sizeX_/2 and
       -sizeY_/2 <= y and y <= sizeY_/2))
    {
        return c;
    }

    int px = int(((x+sizeX_/2)*image_->w)/sizeX_);
    int py = int(((-y+sizeY_/2)*image_->h)/sizeY_);

    Uint32 pix = getpixel(image_, px, py);
    Uint8 r, g, b;
    SDL_GetRGB(pix, image_->format, &r, &g, &b);
    c.r_ = double(r)/255.0;
    c.g_ = double(g)/255.0;
    c.b_ = double(b)/255.0;
    return c;
}

void Image::switchColors()
{
    return;
}

void Image::setColor(const Vector3RGB& c)
{
    return;
}
