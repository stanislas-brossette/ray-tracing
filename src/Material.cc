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
    : rugosity_(mData->rugosity),
      refractiveIndex_(mData->refraction),
      reflectiveness_(mData->reflectiveness),
      lightEmitter_(mData->lightEmitter),
      lightIntensity_(mData->lightIntensity)
{
    if(SolidColorData* tData = dynamic_cast<SolidColorData*>(mData->textureData))
        texture_ = new SolidColor(tData->color);
    else if(CheckerBoardData* tData = dynamic_cast<CheckerBoardData*>(mData->textureData))
        texture_ = new CheckerBoard(tData->color0, tData->color1, tData->squareSizeX, tData->squareSizeY);
    else
        std::cout << "Unrecognized texture" << std::endl;
}

//Material::Material (const Vector3RGB& color, double rugosity, double refIndex, double reflectiveness, bool lightEmitter, double lightIntensity)
//    : color_(color),
//      rugosity_(rugosity),
//      refractiveIndex_(refIndex),
//      reflectiveness_(reflectiveness),
//      lightEmitter_(lightEmitter),
//      lightIntensity_(lightIntensity)
//{
//}

Material::~Material()
{
}

std::string Material::describe() const
{
    std::stringstream ss;
    ss << "texture: " << "\n";
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
