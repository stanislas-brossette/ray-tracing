#include "Item.hh"

Item::Item ()
    : geometry_(nullptr),
      material_(nullptr)
{
}

Item::Item (Geometry* g, Material* m)
    : geometry_(g),
      material_(m)
{
}

Item::Item (const ItemData& iData)
{
    load(iData);
}

Item::~Item()
{
}

void Item::load(const ItemData& iData)
{
    name_ = iData.name;
    material_ = new Material(iData.mData);
    if(SphereData* sData = dynamic_cast<SphereData*>(iData.gData))
        geometry_ = new Sphere(sData);
    else if(PlaneData* sData = dynamic_cast<PlaneData*>(iData.gData))
        geometry_ = new Plane(sData);
    else if(PolygonData* sData = dynamic_cast<PolygonData*>(iData.gData))
        geometry_ = new Polygon(sData);
    else if(CylinderData* sData = dynamic_cast<CylinderData*>(iData.gData))
        geometry_ = new Cylinder(sData);
    else if(ClosedCylinderData* sData = dynamic_cast<ClosedCylinderData*>(iData.gData))
        geometry_ = new ClosedCylinder(sData);
    else if(CubeData* sData = dynamic_cast<CubeData*>(iData.gData))
        geometry_ = new Cube(sData);
    else if(MeshData* sData = dynamic_cast<MeshData*>(iData.gData))
        geometry_ = new Mesh(sData);
    else
        std::cout << "Unrecognized geometry" << std::endl;
}

bool Item::intersect(const LightRay& incident, Vector3& point, Vector3& normal, double& dist, bool verbose) const
{
    PerformanceTracker::instance().incrementCallToIntersect();
    bool intersection = geometry_->intersect(incident, point, normal, dist, verbose);
    if(intersection)
        PerformanceTracker::instance().incrementIntersections();
    return intersection;
}

bool Item::isInHalfSpace(const Vector3& point, const Vector3& normal, const Vector3& specDir, double& cosAngleDiffuse, double& cosAnglePhong) const
{
    bool inHS = geometry_->isInHalfSpace(point, normal, specDir, cosAngleDiffuse, cosAnglePhong);
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

std::string ItemData::describe() const
{
    std::stringstream ss;
    ss << "=== ItemData ===\n";
    ss << "name: " << name << "\n";
    ss << mData->describe() << "\n";
    ss << gData->describe() << "\n";
    return ss.str();
}
