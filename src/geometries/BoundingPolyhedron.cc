#include "geometries/BoundingPolyhedron.hh"

BoundingPolyhedron::BoundingPolyhedron(const Frame3& f)
    : f_(f),
    planeNormals_{
        {1,0,0},
        {0,1,0},
        {0,0,1},
        {0.57735026919,0.57735026919,0.57735026919},
        {-0.57735026919,0.57735026919,0.57735026919},
        {0.57735026919,-0.57735026919,0.57735026919},
        {-0.57735026919,-0.57735026919,0.57735026919}
    },
    pMin_(planeNormals_.size(), INFINITY_d()),
    pMax_(planeNormals_.size(), -INFINITY_d())
{
}

bool BoundingPolyhedron::intersect(const LightRay& incident, Vector3& point, Vector3& normal, double& dist) const
{
    double t0 = -INFINITY_d();
    double t1 = INFINITY_d();
    size_t impactIndex;
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
        {
            t0 = tMin;
            impactIndex = i;
        }
        if(tMax < t1)
            t1 = tMax;
    }
    if(0 < t0 and t0 <= t1) //lr coming from outside intersecting with t0
    {
        point = incident.origin_ + incident.dir_ * t0;
        dist = t0;
        normal = planeNormals_[impactIndex]*(-1);
        return true;
    }
    else if(t0 <= 0 and t1 > 0) //lr coming from incide intersecting with t1
    {
        point = incident.origin_ + incident.dir_ * t1;
        dist = t1;
        normal = planeNormals_[impactIndex];
        return true;
    }
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
