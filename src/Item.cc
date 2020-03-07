#include "Item.hh"

Item::Item (Geometry* g, Material* m)
    : geometry_(g),
      material_(m)
{
}

Item::~Item()
{
}

bool Item::intersect(const LightRay& incident, Vector3& point, Vector3& normal, double& dist) const
{
    return geometry_->intersect(incident, point, normal, dist);
}

bool Item::isInHalfSpace(const Vector3& point, const Vector3& normal, double& cosAngle) const
{
    bool inHS = geometry_->isInHalfSpace(point, normal, cosAngle);
    return inHS;
}

std::string Item::describe() const
{
    std::stringstream ss;
    ss << "Item\n";
    ss << geometry_->describe() << "\n";
    ss << material_->describe();
    return ss.str();
}
