#include "Cube.hh"

Cube::Cube()
    : Geometry(),
    size_(1.0),
    topPlane_(),
    bottomPlane_(),
    frontPlane_(),
    backPlane_(),
    rightPlane_(),
    leftPlane_()
{
}

Cube::Cube(CubeData* cData)
    : Geometry(cData),
    size_(cData->size),
    topPlane_(),
    bottomPlane_(),
    frontPlane_(),
    backPlane_(),
    rightPlane_(),
    leftPlane_()
{
    bv_ = BoundingVolume(f_, size_/0.57);
    initPlanes();
}

Cube::Cube(const Frame3& f, double size)
  : Geometry(f),
    size_(size),
    topPlane_(),
    bottomPlane_(),
    frontPlane_(),
    backPlane_(),
    rightPlane_(),
    leftPlane_()
{
    bv_ = BoundingVolume(f_, size_/0.57);
    initPlanes();
}

void Cube::initPlanes()
{
    std::vector<Vector2> points(4);
    points[0] = Vector2(-size_/2, -size_/2);
    points[1] = Vector2(-size_/2, size_/2);
    points[2] = Vector2(size_/2, size_/2);
    points[3] = Vector2(size_/2, -size_/2);

    Frame3 fLP(f_.o_ - f_.vx_ * (size_/2), f_.vz_, f_.vy_, f_.vx_*(-1));
    leftPlane_ = Polygon(fLP, points);

    Frame3 fRP(f_.o_ + f_.vx_ * (size_/2), f_.vz_*(-1), f_.vy_, f_.vx_); 
    rightPlane_ = Polygon(fRP, points);

    Frame3 fBP(f_.o_ - f_.vy_ * (size_/2), f_.vx_, f_.vz_, f_.vy_*(-1)); 
    backPlane_ = Polygon(fBP, points);

    Frame3 fFP(f_.o_ + f_.vy_ * (size_/2), f_.vx_, f_.vz_*(-1), f_.vy_); 
    frontPlane_ = Polygon(fFP, points);

    Frame3 fBotP(f_.o_ - f_.vz_ * (size_/2), f_.vx_, f_.vy_*(-1), f_.vz_*(-1)); 
    bottomPlane_ = Polygon(fBotP, points);

    Frame3 fTP(f_.o_ + f_.vz_ * (size_/2), f_.vx_, f_.vy_, f_.vz_); 
    topPlane_ = Polygon(fTP, points);
}

Cube::~Cube()
{
}

std::string Cube::describe() const
{
    std::stringstream ss;
    ss << "=== Cube ===\n";
    ss << "center: " << f_.o_ << "\n";
    ss << "size: " << size_ << "\n";
    return ss.str();
}

bool Cube::intersect(const LightRay& lr, Vector3& impactPoint, Vector3& normal, double& dist) const
{
    if(not bv_.intersect(lr))
        return false;

    Vector3 impactPointTop, impactPointBottom, impactPointRight, impactPointLeft, impactPointFront, impactPointBack;
    Vector3 normalTop, normalBottom, normalRight, normalLeft, normalFront, normalBack;

    bool impact = false;

    double distTop = 1e9;
    bool impactTop = topPlane_.intersect(lr, impactPointTop, normalTop, distTop);
    if(not impactTop)
        distTop = 1e9;
    else
        impact = true;

    double distBottom = 1e9;
    bool impactBottom = bottomPlane_.intersect(lr, impactPointBottom, normalBottom, distBottom);
    if(not impactBottom)
        distBottom = 1e9;
    else
        impact = true;

    double distRight = 1e9;
    bool impactRight = rightPlane_.intersect(lr, impactPointRight, normalRight, distRight);
    if(not impactRight)
        distRight = 1e9;
    else
        impact = true;

    double distLeft = 1e9;
    bool impactLeft = leftPlane_.intersect(lr, impactPointLeft, normalLeft, distLeft);
    if(not impactLeft)
        distLeft = 1e9;
    else
        impact = true;

    double distFront = 1e9;
    bool impactFront = frontPlane_.intersect(lr, impactPointFront, normalFront, distFront);
    if(not impactFront)
        distFront = 1e9;
    else
        impact = true;

    double distBack = 1e9;
    bool impactBack = backPlane_.intersect(lr, impactPointBack, normalBack, distBack);
    if(not impactBack)
        distBack = 1e9;
    else
        impact = true;

    if(not impact)
        return false;

    dist = 1e9;
    if(impactTop and distTop < dist)
    {
        dist = distTop;
        normal = normalTop;
        impactPoint = impactPointTop;
    }
    if(impactBottom and distBottom < dist)
    {
        dist = distBottom;
        normal = normalBottom;
        impactPoint = impactPointBottom;
    }
    if(impactRight and distRight < dist)
    {
        dist = distRight;
        normal = normalRight;
        impactPoint = impactPointRight;
    }
    if(impactLeft and distLeft < dist)
    {
        dist = distLeft;
        normal = normalLeft;
        impactPoint = impactPointLeft;
    }
    if(impactFront and distFront < dist)
    {
        dist = distFront;
        normal = normalFront;
        impactPoint = impactPointFront;
    }
    if(impactBack and distBack < dist)
    {
        dist = distBack;
        normal = normalBack;
        impactPoint = impactPointBack;
    }
    return impact;
}

bool Cube::isInHalfSpace(const Vector3& point, const Vector3& normal, double& cosAngle) const
{
    return false;
    // v = OC - r.normal
    //Vector3 v = f_.o_ - point;// - normal*radius_;
    //v.normalize();
    //cosAngle = v.dot(normal);
    //bool inHalfSpace = (cosAngle > 0);
    //return inHalfSpace;
}


std::string CubeData::describe() const
{
    std::stringstream ss;
    ss << "=== Cube ===\n";
    ss << GeometryData::describe();
    ss << "size: " << size << "\n";
    return ss.str();
}
