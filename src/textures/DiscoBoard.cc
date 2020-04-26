#include "textures/DiscoBoard.hh"

DiscoBoard::DiscoBoard(double squareSizeX, double squareSizeY)
    : Texture(),
    colors_{
        {15, 192, 252},
        {123, 29, 175},
        {255, 47, 185},
        {212, 255, 71},
        {27, 54, 73}
    },
    squareSizeX_(squareSizeX),
    squareSizeY_(squareSizeY),
    borderSize_(0.1),
    switchedColors_(0)
{
    for (size_t j = 0; j < colors_.size(); j++)
    {
        colors_[j] *= (1.0/255.0);
    }
    size_t baseColorSize = colors_.size();
    for (size_t i = 0; i < 3; i++)
    {
        for (size_t j = 0; j < baseColorSize; j++)
        {
            colors_.push_back(colors_[j]);
        }
    }
    switchColors();
}

Vector3RGB DiscoBoard::color(double x, double y) const
{
    size_t colorIndex = 0;
    colorIndex = std::floor(x/squareSizeX_) + std::floor(y/squareSizeY_)*(7+colors_.size());

    double intPart;
    double decimalX = std::modf(x/squareSizeX_, &intPart);
    double decimalY = std::modf(y/squareSizeY_, &intPart);

    if(decimalX > -borderSize_ or decimalY > -borderSize_)
        return Vector3RGB(0,0,0);
    else
        return colors_[(colorIndex+switchedColors_)%colors_.size()];
}

void DiscoBoard::switchColors()
{
    std::random_shuffle(colors_.begin(), colors_.end());
}

void DiscoBoard::setColor(const Vector3RGB& c)
{
    return;
}
