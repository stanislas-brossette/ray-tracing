#pragma once
#include <string>
#include <sstream>
#include "Vector3.hh"

class Frame3
{
public:
    Frame3 ();
    Frame3 (const Vector3& o, const Vector3& vx, const Vector3& vy, const Vector3& vz);
    //Translations and rotations are always made w.r.t the world frame
    void translate(const Vector3& v);
    void translate(double x, double y, double z);
    void translateLocal(double x, double y, double z);
    void setOriginPos(double x, double y, double z);
    //angle is given in degrees
    //rotate is always done around the origin of the frame, which therefore does not move with rotate
    void rotate(const Vector3& axis, double angle);
    void roll(double theta); // rotation around frames y axis
    void pitch(double theta); // rotation around frames x axis
    void yaw(double theta); // rotation around frames z axis

    bool operator==(const Frame3& f);
    bool isApprox(const Frame3& f, double prec);

    std::string describe() const;
    friend std::ostream& operator<<(std::ostream& os, const Frame3& f);

    virtual ~Frame3 ();

    Vector3 vecToWorld(const Vector3& v) const;
    Vector3 pointToWorld(const Vector3& v) const;
    Vector3 vecFromWorld(const Vector3& v) const;
    Vector3 pointFromWorld(const Vector3& v) const;

    Vector3 o_, vx_, vy_, vz_;
};
