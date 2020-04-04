#include "Material.hh"

Material::Material()
    : texture_(),
      rugosity_(0),
      refractiveIndex_(0),
      reflectiveness_(0),
      lightEmitter_(false),
      lightIntensity_(0)
{
}

Material::Material (MaterialData* mData)
    : specularExponent_(mData->specularExponent),
      specularGain_(mData->specularGain),
      rugosity_(mData->rugosity),
      refractiveIndex_(mData->refraction),
      absorption_(mData->absorption),
      reflectiveness_(mData->reflectiveness),
      lightEmitter_(mData->lightEmitter),
      lightIntensity_(mData->lightIntensity)
{
    if(SolidColorData* tData = dynamic_cast<SolidColorData*>(mData->textureData))
        texture_ = new SolidColor(tData->color);
    else if(CheckerBoardData* tData = dynamic_cast<CheckerBoardData*>(mData->textureData))
        texture_ = new CheckerBoard(tData->color0, tData->color1, tData->squareSizeX, tData->squareSizeY);
    else if(ImageData* tData = dynamic_cast<ImageData*>(mData->textureData))
        texture_ = new Image(tData->path, tData->sizeX, tData->sizeY);
    else
        std::cout << "Unrecognized texture" << std::endl;
}

Material::~Material()
{
}

std::string Material::describe() const
{
    std::stringstream ss;
    ss << "texture: " << "\n";
    ss << "specularExponent: " << specularExponent_ << "\n";
    ss << "specularGain: " << specularGain_ << "\n";
    ss << "rugosity: " << rugosity_ << "\n";
    ss << "refractiveIndex: " << refractiveIndex_ << "\n";
    ss << "reflectiveness: " << reflectiveness_ << "\n";
    ss << "lightEmitter: " << lightEmitter_ << "\n";
    ss << "lightIntensity: " << lightIntensity_;
    return ss.str();
}

std::string MaterialData::describe() const
{
    std::stringstream ss;
    ss << "=== MaterialData ===\n";
    ss << "texture: " << "\n";
    ss << "specularExponent: " << specularExponent << "\n";
    ss << "specularGain: " << specularGain << "\n";
    ss << "rugosity: " << rugosity << "\n";
    ss << "refractiveIndex: " << refraction << "\n";
    ss << "reflectiveness: " << reflectiveness << "\n";
    ss << "lightEmitter: " << lightEmitter << "\n";
    ss << "lightIntensity: " << lightIntensity;
    return ss.str();
}

std::ostream& operator<<(std::ostream& os, const Material& m)
{
    os << m.describe();
    return os;
}
