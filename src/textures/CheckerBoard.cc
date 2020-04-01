#include "textures/CheckerBoard.hh"

CheckerBoard::CheckerBoard(const Vector3RGB& color0, const Vector3RGB color1, double squareSizeX, double squareSizeY)
    : Texture(),
    color0_(color0),
    color1_(color1),
    squareSizeX_(squareSizeX),
    squareSizeY_(squareSizeY)
{
}

Vector3RGB CheckerBoard::color(double x, double y) const
{
    if((std::remainder(x, squareSizeX_) < 0 and std::remainder(y, squareSizeY_) < 0)
            or (std::remainder(x, squareSizeX_) > 0 and std::remainder(y, squareSizeY_) > 0))
    {
        return color0_;
    }
    else
    {
        return color1_;
    }
}
