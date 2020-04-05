#include "geometries/BoundingPolyhedron.hh"

BoundingPolyhedron::BoundingPolyhedron(const Frame3& f)
    : f_(f),
    planeNormals_{ {1,0,0}, {0,1,0}, {0,0,1} },
    pMin_(planeNormals_.size(), INFINITY_d()),
    pMax_(planeNormals_.size(), -INFINITY_d())
{
}

bool BoundingPolyhedron::intersect(const LightRay& incident) const
{
    double t0 = -INFINITY_d();
    double t1 = INFINITY_d();
    for (size_t i = 0; i < planeNormals_.size(); i++)
    {
        double tMin = (-pMin_[i] - planeNormals_[i].dot(incident.origin_-f_.o_))/(planeNormals_[i].dot(incident.dir_));
        double tMax = (-pMax_[i] - planeNormals_[i].dot(incident.origin_-f_.o_))/(planeNormals_[i].dot(incident.dir_));
        if(tMax < tMin)
        {
            double tmp = tMin;
            tMin = tMax;
            tMax = tmp;
        }
        if(tMin > t0)
            t0 = tMin;
        if(tMax < t1)
            t1 = tMax;
    }
    if(t0 <= t1)
        return true;
    else
        return false;
}

void BoundingPolyhedron::extendBy(const Vector3& point)
{
    for (size_t i = 0; i < planeNormals_.size(); i++)
    {
        double dot = -point.dot(planeNormals_[i]);
        if(dot < pMin_[i])
            pMin_[i] = dot;
        else if(dot > pMax_[i])
            pMax_[i] = dot;
    }
}

BoundingPolyhedron::~BoundingPolyhedron()
{
}

std::string BoundingPolyhedron::describe() const
{
    std::stringstream ss;
    ss << "BoundingPolyhedron:\n";
    for (size_t i = 0; i < planeNormals_.size(); i++)
    {
        ss << "planeNormals_[" << i << "]: " << planeNormals_[i] << "\n";
        ss << "pMin_[" << i << "]: " << pMin_[i] << "\n";
        ss << "pMax_[" << i << "]: " << pMax_[i] << "\n";
    }
    return ss.str();
}
