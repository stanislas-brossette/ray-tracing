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
