#include "textures/CheckerBoard.hh"

CheckerBoard::CheckerBoard(const Vector3RGB& color0, const Vector3RGB color1, double squareSizeX, double squareSizeY)
    : Texture(),
    color0_(color0),
    color1_(color1),
    squareSizeX_(squareSizeX),
    squareSizeY_(squareSizeY),
    reversedColors_(false)
{
}

Vector3RGB CheckerBoard::color(double x, double y) const
{
    if((std::remainder(x, 2*squareSizeX_) < 0 and std::remainder(y, 2*squareSizeY_) < 0)
            or (std::remainder(x, 2*squareSizeX_) > 0 and std::remainder(y, 2*squareSizeY_) > 0))
    {
        if(reversedColors_) return color1_;
        else return color0_;
    }
    else
    {
        if(reversedColors_) return color0_;
        else return color1_;
    }
}

void CheckerBoard::switchColors()
{
    reversedColors_ = not reversedColors_;
}

void CheckerBoard::setColor(const Vector3RGB& c)
{
    return;
}
