#include "Material.hh"

Material::Material()
    : color_(0,0,0),
      rugosity_(0),
      refractionIndex_(0),
      lightEmitter_(false),
      lightIntensity_(0)
{
}

Material::Material (const Vector3RGB color, double rugosity, double refIndex, bool lightEmitter, double lightIntensity)
    : color_(color),
      rugosity_(rugosity),
      refractionIndex_(refIndex),
      lightEmitter_(lightEmitter),
      lightIntensity_(lightIntensity)
{
}

Material::~Material()
{
}

std::string Material::describe() const
{
    std::stringstream ss;
    ss << "color: " << color_ << "\n";
    ss << "rugosity: " << rugosity_ << "\n";
    ss << "refractionIndex: " << refractionIndex_ << "\n";
    ss << "lightEmitter: " << lightEmitter_ << "\n";
    ss << "lightIntensity: " << lightIntensity_;
    return ss.str();
}
