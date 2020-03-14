#include "Material.hh"

Material::Material()
    : color_(0,0,0),
      rugosity_(0),
      refractionIndex_(0),
      reflectiveness_(0),
      lightEmitter_(false),
      lightIntensity_(0)
{
}

Material::Material (const Vector3RGB& color, double rugosity, double refIndex, double reflectiveness, bool lightEmitter, double lightIntensity)
    : color_(color),
      rugosity_(rugosity),
      refractionIndex_(refIndex),
      reflectiveness_(reflectiveness),
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
    ss << "reflectiveness: " << reflectiveness_ << "\n";
    ss << "lightEmitter: " << lightEmitter_ << "\n";
    ss << "lightIntensity: " << lightIntensity_;
    return ss.str();
}

std::string materialData::describe() const
{
    std::stringstream ss;
    ss << "=== materialData ===\n";
    ss << "color: " << color << "\n";
    ss << "rugosity: " << rugosity << "\n";
    ss << "refractionIndex: " << refraction << "\n";
    ss << "reflectiveness: " << reflectiveness << "\n";
    ss << "lightEmitter: " << lightEmitter << "\n";
    ss << "lightIntensity: " << lightIntensity;
    return ss.str();
}
